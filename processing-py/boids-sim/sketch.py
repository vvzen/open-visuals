from vv_trigo import Vector2d
from boids import Boid

boid = None


def setup():
    size(600, 600)
    noStroke()
    global boid
    boid = Boid(width / 2, height / 2, 10, 1)


def draw():
    background(255)

    mouse = Vector2d(mouseX, mouseY)

    #look_at_mouse = Vector2d.subtract(vec1=mouse, vec2=boid.pos)
    # look_at_mouse.normalize()
    # look_at_mouse.mult(30)

    boid.arrive(mouse)

    # boid.apply_force(look_at_mouse)
    boid.wrap_edges(width, height)
    boid.update()

    fill(0)
    boid.render()
