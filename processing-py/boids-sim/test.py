from vv_trigo import Vector2d
from boids import Boid

boid = Boid(512 / 2, 512 / 2, 5, 1)


for i in range(0, 2):
    print("========================")
    print("loop {}".format(i))
    destination = Vector2d(100, 100)
    boid.arrive(destination)
    boid.update()
    boid.wrap_edges()
