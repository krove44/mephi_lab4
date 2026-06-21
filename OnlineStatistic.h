#pragma once
#include "Streams\IReadOnlyStream.h"
#include "mephi_lab2/Sequence/ArraySequence.h"

template<typename T>
class Statistics {
private:
    T sum_ = T(0);
    T min_ = T(0);
    T max_ = T(0);
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
        if (i > 0) {
            quick_sort(arr, low, i - 1);
        }
        quick_sort(arr, i + 1, high);
    }

public:
    Statistics() : sum_(T(0)), min_(T(0)), max_(T(0)), values_() {};

    void add(const T& value) {
        if (values_.GetLenght() == 0) {
            min_ = value;
            max_ = value;
        } else {
            if (value < min_) min_ = value;
            if (value > max_) max_ = value;
        }
        sum_ += value;
        values_.Append(value);
    }

    void read_from_stream(IReadOnlyStream<T>& stream, size_t limit = 0) {
        size_t read_count = 0;
        while (true) {
            if (limit > 0 && read_count >= limit) {
                break;
            }
            auto opt_val = stream.try_read();
            if (!opt_val.has_value()) {
                break;
            }
            add(opt_val.value());
            read_count++;
        }
    }

    void reset() {
        sum_ = T(0);
        min_ = T(0);
        max_ = T(0);
        values_.Clear();
    }

    T sum() const { return sum_; }
    T min() const { return min_; }
    T max() const { return max_; }
    size_t count() const { return values_.GetLenght(); }

    double get_median() {
        size_t len = values_.GetLenght();
        if (len == 0) {
            return 0.0;
        }
        ArraySequence<T> temporary = values_;
        if (len > 0) {
            quick_sort(temporary, 0, len - 1);
        }
        if (len % 2 == 1) {
            return static_cast<double>(temporary.Get(len / 2));
        } 
        else {
            T left = temporary.Get(len / 2 - 1);
            T right = temporary.Get(len / 2);
            return (static_cast<double>(left) + static_cast<double>(right)) / 2.0;
        }
    }
};