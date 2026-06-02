#pragma once
#include "Streams\ReadOnlyStream.h"
#include "Optional.h"
#include "mephi_lab2/Sequence/ArraySequence.h"

template<typename T>
class Statistics {
    private:
        size_t count_;
        T sum_;
        T min_;
        T max_;
        ArraySequence<T> values_;

        size_t partition(ArraySequence<T>& arr, size_t low, size_t high) {
            T pivot = arr.Get(high);
            size_t i = low;
            for (size_t j = low; j < high; j++) {
                if (arr.Get(j) < pivot) {
                    T temporary = arr.Get(i);
                    arr.Set(i, arr.Get(j));
                    arr.Set(j, temporary);
                    i++;
                }
            }
            T temporary = arr.Get(i);
            arr.Set(i, arr.Get(high));
            arr.Set(high, temporary);
            return i;
        }

        void quick_sort(ArraySequence<T>& arr, size_t low, size_t high) {
            if (low >= high) return;
            size_t i = partition(arr, low, high);
            if (i > low) quick_sort(arr, low, i - 1);
            if (i + 1 < high) quick_sort(arr, i + 1, high);
        }

    public:
        Statistics() : count_(0), sum_(0), min_(0), max_(0) {}

        void add(const T& value) {
            if (count_ == 0) {
                min_ = value;
                max_ = value;
            } else {
                if (value < min_) min_ = value;
                if (value > max_) max_ = value;
            }
            sum_ += value;
            values_.PushBack(value);
            count_++;
        }

        void read_from_stream(ReadOnlyStream<T>& stream, size_t limit = 0) {
            size_t count = 0;
            while (!stream.is_end()) {
                if (limit > 0 && count >= limit) break;
                Optional<T> optional_value = stream.try_read();
                if (optional_value.has_value()) {
                    add(optional_value.value());
                    count++;
                }
            }
        }

        void reset() {
            count_ = 0;
            sum_ = T(0);
            min_ = T(0);
            max_ = T(0);
            values_ = ArraySequence<T>();
        }

        T sum() const { 
            return sum_; 
        }

        T min() const {
            return min_; 
        }

        T max() const {
            return max_;
        }

        size_t count() const {
            return count_;
        }

        double get_median() {
            ArraySequence<T> temporary = values_;
            size_t count = temporary.GetLength();
            
            if (count > 0) {
                quick_sort(temporary, 0, count - 1);
            }

            if (count % 2 == 1) {
                return static_cast<double>(temporary.Get(count / 2));
            } 
            else {
                T left = temporary.Get(count / 2 - 1);
                T right = temporary.Get(count / 2);
                return (static_cast<double>(left) + static_cast<double>(right)) / 2.0;
            }
        }
};