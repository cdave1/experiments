from PIL import Image
import math
import numpy

class vec2(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y

    @classmethod
    def from_list(cls, lst):
        x = lst[0]
        y = lst[1]
        return cls(x, y)

    def add(self, b):
        return vec2(self.x + b, self.y + b)

    def add_vec(self, vec):
        return vec2(self.x + vec.x, self.y + vec.y)

    def sub_vec(self, vec):
        return vec2(self.x - vec.x, self.y - vec.y)

    def mult(self, val):
        return vec2(self.x * val, self.y * val)

    def mult_vec(self, vec):
        return vec2(self.x * vec.x, self.y * vec.y)

    def dot(self, vec):
        return self.x * vec.x + self.y * vec.y

    def __str__(self):
        return "Vec2: [{} {}]".format(self.x, self.y)



class vec4(object):
    def __init__(self, x, y, z, w):
        self.x = x
        self.y = y
        self.z = z
        self.w = w

    @classmethod
    def from_list(cls, lst):
        x = lst[0]
        y = lst[1]
        z = lst[2]
        w = lst[3]
        return cls(x, y, z, w)

    def add(self, s):
        return vec4(self.x + s, self.y + s, self.z + s, self.w + s)

    def add_vec(self, vec):
        return vec4(self.x + vec.x, self.y + vec.y, self.z + vec.z, self.w + vec.w)

    def mult(self, val):
        return vec4(self.x * val, self.y * val, self.z + val, self.w + val)

    def dot(self, vec):
        return self.x * vec.x + self.y * vec.y + self.z * vec.z + self.w * vec.w

    def __str__(self):
        return "Vec4: [{} {} {} {}]".format(self.x, self.y, self.z, self.w)


img = Image.open("bg.png").load()

def fragment(iUv, resolution, scale, scale_in, lens_center, hmd_warp_param, chroma_ab_param):
    uv = vec2(iUv.x / float(resolution.x), iUv.y / float(resolution.y))
    uv = uv.mult(2).add(-1)
    theta = uv.sub_vec(lens_center).mult_vec(scale_in)

    rsq = (theta.x * theta.x) + (theta.y * theta.y)
    theta1 = theta.mult(hmd_warp_param.x + (hmd_warp_param.y * rsq) + (hmd_warp_param.z * rsq * rsq) + (hmd_warp_param.w * rsq *rsq *rsq))
    
    theta_blue = theta1.mult(chroma_ab_param.z + chroma_ab_param.w * rsq)
    tcblue = lens_center.add_vec(scale.mult_vec(theta_blue))

    blue_clamped = vec2(numpy.clip(tcblue.x, 0.0, 1.0), numpy.clip(tcblue.y, 0.0, 1.0))
    blue_clamped = blue_clamped.sub_vec(tcblue)
    print blue_clamped
    if (blue_clamped.x > 0.0 or blue_clamped.y > 0.0):
        return (0, 0, 0, 255)


    tcgreen = lens_center.add_vec(scale.mult_vec(theta1))
    tcgreen = tcgreen.add(1).mult(0.5)

    rred = theta1.mult(chroma_ab_param.z + chroma_ab_param.w * rsq)
    tcred = lens_center.add_vec(scale.mult_vec(rred))
    tcred = tcred.add(1).mult(0.5)

    red_pos = tcred.mult_vec(resolution)
    green_pos = tcgreen.mult_vec(resolution)
    blue_pos = tcblue.mult_vec(resolution)

    color = (img[red_pos.x, red_pos.y][0], img[green_pos.x, green_pos.y][1], img[blue_pos.x, blue_pos.y][2], 255)
    return color


class hmd(object):
    def __init__(self):
        self.resolution = vec2(1280, 800)
        self.screen_size = vec2(0.14976, 0.0936)

        self.interpupillary_distance = 0.064 # units?
        self.lens_separation_distance = 0.064

        self.eye_to_screen_distance = 0.041
        self.distortion_k = [1.0, 0.22, 0.24, 0.0]
        self.chroma_ab_parameter = [ 0.996, -0.004, 1.014, 0.0]


    def lens_to_png(self, name, scale, scale_in, lens_center, hmd_warp_param, chroma_ab_param):
        pixels = []
        for y in range(self.resolution.y):
            for x in range(self.resolution.x):
                color = fragment(vec2(x, y), self.resolution, scale, scale_in, lens_center, hmd_warp_param, chroma_ab_param);
                pixels.append(color)
        

        image = Image.new("RGBA", (self.resolution.x, self.resolution.y))
        image.putdata(pixels)
        image.save(name)


    def generate(self):
        aspect = self.resolution.x / (2.0 * self.resolution.y);

        r = -1.0 - (4 * (self.screen_size.x / 4 - self.lens_separation_distance / 2) / self.screen_size.x);
        dist_scale = self.distortion_k[0] + self.distortion_k[1] * pow(r, 2) + self.distortion_k[2] * pow(r, 4) + self.distortion_k[3] * pow(r, 6)
        fov = 2.0 * math.atan2(self.screen_size.y * dist_scale, 2 * self.eye_to_screen_distance);

        lens_shift = 4 * (self.screen_size.x / 4 - self.lens_separation_distance / 2) / self.screen_size.x;
        left_lens_center = vec2(lens_shift, 0);
        right_lens_center = vec2(-lens_shift, 0);

        hmd_warp_param = vec4.from_list(self.distortion_k)
        chroma_ab_param = vec4.from_list(self.chroma_ab_parameter)
        scale = vec2(1.0, 1.0/aspect)
        scale_in = vec2(1.0/dist_scale, 1.0 * aspect/dist_scale)

        self.lens_to_png("left.png", scale, scale_in, left_lens_center, hmd_warp_param, chroma_ab_param)
        self.lens_to_png("right.png", scale, scale_in, right_lens_center, hmd_warp_param, chroma_ab_param)



def start():
    oculus = hmd()
    oculus.generate()


start()
