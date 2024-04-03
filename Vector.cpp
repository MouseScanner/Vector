#include "Vector.h"
#include <cstring>
#include <iostream>

Vector::Vector(const ValueType *rawArray, const size_t size, float coef) {
  if (coef < 2) {
    throw std::invalid_argument("Low coefficient. Must be at least 2.");
  }

  _size = size;
  _capacity = size;
  _multiplicativeCoef = coef;
  _data = {new ValueType[size]};

  for (int i = 0; i < size; i++) {
    _data[i] = rawArray[i];
  }
}

Vector::Vector(const Vector &other) {
  _size = other._size;
  _capacity = other._size;
  _multiplicativeCoef = other._multiplicativeCoef;
  _data = {new ValueType[_capacity]};

  for (int i = 0; i < _size; i++) {
    _data[i] = other._data[i];
  }
}

Vector &Vector::operator=(const Vector &other) {
  if (this != &other) {
    _size = other._size;
    _capacity = other._capacity;
    _multiplicativeCoef = other._multiplicativeCoef;
    ValueType *newData = {new ValueType[_capacity]};

    for (int i = 0; i < _size; i++) {
      _data[i] = other._data[i];
    }

    delete[] _data;
    _data = newData;
  }

  return *this;
}

Vector::Vector(Vector &&other) noexcept {
  _size = other._size;
  _capacity = other._capacity;
  _data = other._data;
  _multiplicativeCoef = other._multiplicativeCoef;

  other._data = nullptr;
  other._size = 0;
  other._capacity = 0;
}

Vector &Vector::operator=(Vector &&other) noexcept {
  if (this != &other) {
    _size = other._size;
    _capacity = other._capacity;
    _multiplicativeCoef = other._multiplicativeCoef;

    delete[] _data;
    _data = other._data;

    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
  }

  return *this;
}

Vector::~Vector() { delete[] _data; };

void Vector::pushBack(const ValueType &value) {
  if (_size + 1 >= _capacity) {
    if (_capacity == 0) {
      _capacity++;
    }

    while (_size + 1 >= _capacity) {
      _capacity *= _multiplicativeCoef;
    }

    ValueType *newData = new ValueType[_capacity];
    for (int i = 0; i < _size; i++) {
      newData[i] = _data[i];
    }
    delete[] _data;
    _data = newData;
  }

  _data[_size] = value;
  _size++;
};

void Vector::pushFront(const ValueType &value) {
  if (_size + 1 >= _capacity) {
    if (_capacity == 0) {
      _capacity++;
    }

    while (_size + 1 >= _capacity) {
      _capacity *= _multiplicativeCoef;
    }
    ValueType *newData = new ValueType[_capacity];
    for (int i = 0; i < _size; i++) {
      newData[i] = _data[i];
    }
    delete[] _data;
    _data = newData;
  }

  for (int i = _size; i > 0; i--) {
    _data[i] = _data[i - 1];
  }

  _data[0] = value;
  _size++;
};

void Vector::insert(const ValueType &value, size_t pos) {
  if (_size + 1 >= _capacity) {
    if (_capacity == 0)
      _capacity = 1;

    _capacity *= _multiplicativeCoef;

    ValueType *newData = new ValueType[_capacity];
    for (int i = 0; i < _size; i++) {
      newData[i] = _data[i];
    }

    delete[] _data;
    _data = newData;
  }

  if (pos > _size) {
    _data[_size] = value;
  } else {
    for (int i = _size; i > pos; i--) {
      _data[i] = _data[i - 1];
    }
    _data[pos] = value;
  }

  _size++;
}

void Vector::insert(const ValueType *values, size_t size, size_t pos) {
  if (_size + size >= _capacity) {
    if (_capacity == 0)
      _capacity = 1;

    _capacity = (_capacity * _multiplicativeCoef) + size;
  }

  ValueType *newData = new ValueType[_capacity];
  for (int i = 0; i < _size; i++) {
    newData[i] = _data[i];
  }

  delete[] _data;
  _data = newData;

  if (pos > _size) {
    for (int i = _size; i > pos; i--) {
      _data[i + _size] = values[i];
    }
  } else {
    for (int i = _size; i > pos; i--) {
      _data[i + size - 1] = _data[i - 1];
    }

    for (int i = 0; i < size; i++) {
      _data[i + pos] = values[i];
    }
  }

  _size += size;
}

void Vector::popBack() {
  if (_size == 0) {
    throw "Nothing to pop. Current size equals to 0.";
  }

  _size--;
};

void Vector::popFront() {
  if (_size == 0) {
    throw "Nothing to pop. Current size equals to 0.";
  }

  for (int i = 0; i < _size - 1; i++) {
    _data[i] = _data[i + 1];
  }

  _size--;
};

void Vector::erase(size_t pos, size_t count) {
  size_t leftToEnd = _size - pos;

  count = count > leftToEnd ? leftToEnd : count;

  for (size_t i = pos; i < _size - count; ++i) {
    _data[i] = _data[i + count];
  }

  _size -= count;
};

void Vector::eraseBetween(size_t beginPos, size_t endPos) {
  erase(beginPos, endPos - beginPos);
};

size_t Vector::size() const { return _size; };

size_t Vector::capacity() const { return _capacity; };

double Vector::loadFactor() const { return _size / _capacity; };

ValueType &Vector::operator[](size_t idx) { return _data[idx]; };

const ValueType &Vector::operator[](size_t idx) const { return _data[idx]; };

long long Vector::find(const ValueType &value) const {
  for (long long i = 0; i < _size; i++) {
    if (_data[i] == value)
      return i;
  }

  return -1;
};

void Vector::reserve(size_t capacity) {
  if (capacity > _capacity) {
    _capacity = capacity;

    ValueType *newData = new ValueType[_capacity];
    for (int i = 0; i < _size; i++) {
      newData[i] = _data[i];
    }

    delete[] _data;
    _data = newData;
  }
};

void Vector::shrinkToFit() {
  _capacity = _size;
  ValueType *newData = new ValueType[_capacity];

  for (int i = 0; i < _size; ++i) {
    newData[i] = _data[i];
  }

  delete[] _data;
  _data = newData;
};

Vector::Iterator::Iterator(ValueType *ptr) : _ptr(ptr) {}

ValueType &Vector::Iterator::operator*() { return *_ptr; }

const ValueType &Vector::Iterator::operator*() const { return *_ptr; }

ValueType *Vector::Iterator::operator->() { return _ptr; }

const ValueType *Vector::Iterator::operator->() const { return _ptr; }

Vector::Iterator Vector::Iterator::operator++() {
  _ptr++;
  return *this;
}

Vector::Iterator Vector::Iterator::operator++(int) {
  Iterator temp = *this;
  _ptr++;
  return temp;
}

bool Vector::Iterator::operator==(const Iterator &other) const {
  return _ptr == other._ptr;
}

bool Vector::Iterator::operator!=(const Iterator &other) const {
  return !(*this == other);
}

Vector::Iterator Vector::begin() {
  Iterator iterator(_data);
  return iterator;
};

Vector::Iterator Vector::end() {
  Iterator iterator(_data + _size);
  return iterator;
};