class Slice:
    def __init__(self, s = None):
        if s:
            self.capacity = s.capacity
            self.length = s.length
            self.data = s.data
        else:
            self.capacity = self.length = 0
            self.data = []
    def add(self, item):
        new = Slice(self)
        if new.length >= new.capacity:
            new.capacity *= 2
            new.data = new.data[:]
        new.data.append(item)
        new.length += 1
        return new
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
class Array:
    def __init__(self, data):
        self.data = data
        self.capacity = len(data)
    def __eq__(self, other):
        return all(i == j for i,j in zip(self.data, other.data))
    def __len__(self):
        return self.capacity
    def __getitem__(self, key):
        if not 0 <= key < self.length:
            raise IndexError('array index out of range')
        return self.data[key]
    def __setitem__(self, key, value):
        if not 0 <= key < self.length:
            raise IndexError('array assignment index out of range')
        self.data[key] = value
switchvals = []
