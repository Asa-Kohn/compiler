import numpy
class Slice:
    def __init__(self, data = None, length = 0):
        if data:
            self.data = data
            self.capacity = data.capacity
        else:
            self.data = Array()
            self.capacity = 0
        self.length = length
    def __len__(self):
        return self.length
    def __getitem__(self, key):
        if not 0 <= key < self.length:
            raise IndexError('slice index out of range')
        return self.data[key]
    def __setitem__(self, key, value):
        if not 0 <= key < self.length:
            raise IndexError('slice assignment index out of range')
        self.data[key] = value
    def add(self, item):
        if self.length >= self.capacity:
            if self.capacity == 0:
                self.capacity = 2
            else:
                self.capacity *= 2
            newarr = Array(length = self.capacity, gen = lambda: None)
            for i in range(self.length):
                newarr[i] = self.data.data[i]
            self.data = newarr
        self.data.data[self.length] = item
        self.length += 1
        return self
    def copy(self):
        return Slice(self.data, self.length)
class Array:
    def __init__(self, data = None, length = 0, gen = lambda: 0):
        if data is not None:
            self.data = data
            self.capacity = len(data)
        else:
            self.data = numpy.zeros(length, dtype = numpy.dtype(numpy.object))
            for i in range(length):
                self.data[i] = gen()
            self.capacity = length
    def __eq__(self, other):
        return all(i == j for i,j in zip(self.data, other.data))
    def __len__(self):
        return self.capacity
    def __getitem__(self, key):
        if key < 0:
            raise IndexError('array index out of range')
        return self.data[key]
    def __setitem__(self, key, value):
        if key < 0:
            raise IndexError('array assignment index out of range')
        self.data[key] = value
    def copy(self):
        try:
            data = numpy.zeros(self.capacity)
            for i in range(self.capacity):
                data[i] = self.data[i]
            return Array(data = data)
        except AttributeError:
            return Array(data = self.data.copy())
class Struct(dict):
    def copy(self):
        new = {}
        for i in self:
            try:
                new[i] = self[i].copy()
            except AttributeError:
                new[i] = self[i]
        return Struct(new)
def format(x):
    if type(x) is int:
        return str(x)
    if type(x) is bool:
        return 'true' if x else 'false'
    if type(x) is float:
        s = '{:+e}'.format(x)
        if s[-3] == '+':
            s = s[:-2] + '0' + s[-2:]
        return s
    return x
switchvals = []
