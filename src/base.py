from copy import deepcopy
class Slice:
    __slots__ = ('capacity', 'length', 'data')
    def __init__(self, s = None):
        if s:
            self.capacity = s.capacity
            self.length = s.length
            self.data = s.data
        else:
            self.capacity = self.length = 0
            self.data = Array()
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
            self.data = deepcopy(self.data)
        self.data.append(item)
        self.length += 1
        return self
    def __deepcopy__(self, memo):
        return Slice(self)
class Array(list):
    def __init__(self, data = None, length = 0, gen = lambda: 0):
        if data is not None:
            list.__init__(self, data)
        else:
            list.__init__(self, [gen() for i in range(length)])
    def __getitem__(self, key):
        if key < 0:
            raise IndexError('array index out of range')
        return list.__getitem__(self, key)
    def __setitem__(self, key, value):
        if key < 0:
            raise IndexError('array assignment index out of range')
        return list.__setitem__(self, key, value)
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
