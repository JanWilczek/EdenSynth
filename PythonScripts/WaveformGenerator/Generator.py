#!/usr/bin/env python
__author__ = "Jan Wilczek"
__date__ = "22.10.2018"

from abc import ABC, abstractmethod


class Generator(ABC):

    @abstractmethod
    def generate(self, length):
        pass

    @abstractmethod
    def name(self):
        pass
