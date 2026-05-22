//взял с гугл папки, это не нейронка!!!

#pragma once

#include <memory>
#include "Optional.hpp"
#include "Sequence.hpp"

using namespace std;

template<typename T>
class ILazySequenceIterator;

template<typename T>
class LazySequence
{
private:
	class IGenerator;
	class DefaultGenerator;
	class SkipGenerator;
protected:
	unique_ptr<Sequence<T>> items;//materializedItems
	IGenerator* generator;
public:
	LazySequence(unique_ptr<Sequence<T>> startItems, T(*generator)(T))// std::function<>
	{
		//
		//this->generator = UnaryFunctionGenerator(1, generator);
	}
	LazySequence(unique_ptr<Sequence<T>> startItems, T(*generator)(T,T))// std::function<>
	{
		//
		//this->generator = UnaryFunctionGenerator(2, generator);
	}
	LazySequence(unique_ptr<Sequence<T>> startItems, size_t generatorArity, T (*generator)(Sequence<T>*))// std::function<>
	{
		this->items = move(startItems);
		this->generator = (IGenerator*)new DefaultGenerator(shared_ptr<LazySequence<T>>(this), generatorArity, generator);
	}
private:
	LazySequence(size_t startSkip, size_t endSkip, shared_ptr<LazySequence<T>> parentSeq)
	{
		this->items = make_unique<Sequence<T>>(new ArraySequence<T>());
		this->generator = (IGenerator*)new SkipGenerator(shared_ptr<LazySequence<T>>(this), startSkip, endSkip, parentSeq);
	}
public:
	int GetLength() const
	{
		return this->length;
	}
	bool IsEmpty()
	{
		return this->length == 0;
	}
public:
	virtual T Get(size_t index)
	{
		if (index >= this->items->GetLength())
		{
			for (auto i = this->items->GetLength() - 1; i <= index; i++;)
			{
				this->items->Append(this->generator->GetNext());
			}
		}
		return this->items->Get(index);
	};
	virtual T& operator[](size_t index) = 0;
	virtual T GetFirst() const = 0;
	virtual T GetLast() const = 0;
	virtual Sequence<T>* GetFirst(size_t count) const = 0;
	virtual Sequence<T>* GetLast(size_t count) const = 0;
	virtual LazySequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const = 0;
	virtual LazySequence<T>* Append(LazySequence<T>* seq) = 0;
	virtual LazySequence<T>* Prepend(LazySequence<T>* seq) = 0;
	virtual LazySequence<T>* InsertAt(size_t index, LazySequence<T>*) = 0;
	virtual LazySequence<T>* SkipFirst(size_t count) const = 0;
	virtual LazySequence<T>* Skip(size_t start, size_t end)
	{
		return new LazySequence<T>(start, end, shared_ptr<LazySequence<T>>(this));
	}
	virtual LazySequence<T>* Skip(shared_ptr<LazySequence<size_t>> indexes) = 0;
public:
	ILazySequenceIterator<T>* GetIterator();
public:
	template<typename T2>
	virtual LazySequence<T2>* Map(T2(*)(T));
	template<typename T2>
	virtual T2 Reduce(T2(*)(T2, T)) = 0;
	template<typename T2>
	virtual T2 Fold(T2(*)(T2, T), T2 init) = 0;
	virtual LazySequence<T>* Where(bool(*)(T)) = 0;
	template<typename T2>
	virtual LazySequence<Pair<T, T2>>* Zip(LazySequence<T2>*) = 0;
public:
	virtual ~LazySequence() = default;
private:
	template<typename T>
	class LazySequenceIterator : ILazySequenceIterator
	{
		virtual T GetNext() override
		{
		}
		virtual bool HasNext() const override;
		virtual Optional<T> TryGetNext() override;
	};
private:
	template<typename T>
	class LazySequenceIterator : ILazySequenceIterator<T>
	{
	public:
		virtual size_t GetCurrentIndex() const = 0;
		virtual T GetNext() = 0;
		virtual bool HasNext() const = 0;
		virtual Optional<T> TryGetNext() = 0;
	}; 
private:
	class IGenerator
	{
	public:
		virtual T GetNext() = 0;
		virtual bool HasNext() const = 0;
		virtual Optional<T> TryGetNext() = 0;
	public:
		virtual shared_ptr<IGenerator> Append(shared_ptr<LazySequence<T>> seq) = 0;
		virtual shared_ptr<IGenerator> Skip(shared_ptr<LazySequence<T>> seq) = 0;
		virtual shared_ptr<IGenerator> Insert(shared_ptr<LazySequence<T>> seq) = 0;

	};

	class DefaultGenerator : public IGenerator
	{
	private:
		weak_ptr<LazySequence<T>> owner;
		size_t generatorArity;
		T(*generator)(Sequence<T>*);
	public:
		DefaultGenerator(shared_ptr<LazySequence> owner, size_t generatorArity, T(*generator)(Sequence<T>*))
		{
			this->owner = owner;
			this->generator = generator;
		}
	public:
		virtual T GetNext() override
		{
			Sequence<T>* prevItems = this->owner->items->GetLast(this->owner->generatorArity);
			return generator(prevItems);
		}
		virtual bool HasNext() const override;
		virtual Optional<T> TryGetNext() override;
	public:
		virtual shared_ptr<IGenerator> Append(shared_ptr<LazySequence<T>> seq) override;
		virtual shared_ptr<IGenerator> Skip(shared_ptr<LazySequence<T>> seq) override;
		virtual shared_ptr<IGenerator> Insert(shared_ptr<LazySequence<T>> seq) override;
	};

	class SkipGenerator : IGenerator
	{
	private:
		shared_ptr<LazySequence> owner;
		size_t startSkip;
		size_t endSkip;
		shared_ptr<LazySequence> parentSeq;
		ILazySequenceIterator<T>* last;
	public:
		SkipGenerator(shared_ptr<LazySequence<T>> owner, size_t startSkip, size_t endSkip, shared_ptr<LazySequence<T>> parentSeq)
		{
			this->owner = owner;
			this->startSkip = startSkip;
			this->endSkip = endSkip;
			this->parentSeq = parentSeq;
			this->last = this->parentSeq->GetIterator();
			//this->last->GetNext();
		}
	public:
		virtual T GetNext() override
		{
			if (this->startSkip <= this->last->GetCurrentIndex() && this->last->GetCurrentIndex() <= this->endSkip)
				for (auto i = this->startSkip; i < this->endSkip; i++)
					this->last->TryGetNext();
			return this->last->GetNext();
		}
		virtual bool HasNext() const override;
		virtual Optional<T> TryGetNext() override;
	public:
		virtual shared_ptr<IGenerator> Append(shared_ptr<LazySequence<T>> seq) override;
		virtual shared_ptr<IGenerator> Skip(shared_ptr<LazySequence<T>> seq) override;
		virtual shared_ptr<IGenerator> Insert(shared_ptr<LazySequence<T>> seq) override;
	};
};


template<typename T>
class ILazySequenceIterator
{
public:
	virtual size_t GetCurrentIndex() const = 0;
	virtual T GetNext() = 0;
	virtual bool HasNext() const = 0;
	virtual Optional<T> TryGetNext() = 0;
};