import math


def map(x, in_min, in_max, out_min, out_max):
    ''' a classic function for mapping a value between two intervals, equivalent to houdini fit()'''
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min


class Vector2d:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y
        self.angle = self.compute_angle()
        self.mag = self.get_mag()

    def heading(self, degrees=True):
        self.angle = self.compute_angle()
        if degrees:
            return math.degrees(self.angle)
        else:
            return self.angle

    def set(self, x, y):
        self.x = x
        self.y = y
        self.angle = self.compute_angle()
        self.mag = self.get_mag()()

    def add(self=None, vec1=None, vec2=None):
        if vec2 is None and self is not None:
            self.x += vec1.x
            self.y += vec1.y
        # static
        else:
            x = vec1.x + vec2.x
            y = vec1.y + vec2.y
            return Vector2d(x, y)

    def subtract(self=None, vec1=None, vec2=None):
        print("------------------------")
        print("subtract()")
        print("vec2: {}".format(vec2))
        if vec2 is None and self is not None:
            print("Instance!")
            self.x -= vec1.x
            self.y -= vec1.y
        # static
        else:
            print("Static!")
            x = vec1.x - vec2.x
            y = vec1.y - vec2.y
            return Vector2d(x, y)

    def normalize(self=None, vec=None):

        if vec is None and self is not None:
            if self.get_mag() != 0:
                self.div(self.get_mag())
        # static
        else:
            if vec.get_mag() != 0:
                vec.div(vec.get_mag(vec=vec))

    def mult(self=None, amount=1, vec=None):
        if vec is None and self is not None:
            self.x *= amount
            self.y *= amount
        else:
            vec.x *= amount
            vec.y *= amount

    def div(self=None, amount=1, vec=None):
        if vec is None and self is not None:
            self.x /= amount
            self.y /= amount
        else:
            vec.x /= amount
            vec.y /= amount

    def compute_angle(self=None, point=None):
        if point is None and self is not None:
            math.atan2(self.y, self.x)
        # static
        else:
            return math.atan2(point.y, point.x)

    def get_mag(self=None, vec=None):
        if vec is None and self is not None:
            return math.sqrt(self.compute_mag_sq())
            # return math.hypot(self.x, self.y)
        # static
        else:
            return math.sqrt(vec.compute_mag_sq())

    def compute_mag_sq(self=None, vec=None):
        if vec is None and self is not None:
            return (self.x * self.x) + (self.y * self.y)
            # return math.hypot(self.x, self.y)
        # static
        else:
            return (vec.x * vec.x) + (vec.y * vec.y)

    def limit(self, n):
        if self.get_mag() > n:
            self.normalize()
            self.mult(n)
