#!/usr/bin/env python
__author__ = "Jan Wilczek"
__date__ = "26.10.2018"


def limit(samples):
    samples[samples > 1] = 1
    samples[samples < -1] = -1
