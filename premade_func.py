"""
This is a module of shorthands and premade functions to help write code faster and smoother.
"""

import time
import math
import random
import sys
import os
import keyboard
import pyautogui
from pynput.mouse import Button, Controller
from pynput.mouse import Listener
from concurrent.futures import ThreadPoolExecutor
import mss
from mss import *
import numpy as np
import re
import collections
from collections import *

import math

alphabet_and = [
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "a",
    "b",
    "c",
    "d",
    "e",
    "f",
    "g",
    "h",
    "i",
    "j",
    "k",
    "l",
    "m",
    "n",
    "o",
    "p",
    "q",
    "r",
    "s",
    "t",
    "u",
    "v",
    "w",
    "x",
    "y",
    "z",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "!",
    "~",
    "`",
    "@",
    "#",
    "$",
    "%",
    "^",
    "&",
    "*",
    "(",
    ")",
    "-",
    "_",
    "=",
    "+",
    "[",
    "]",
    "{",
    "}",
    "\\",
    "|",
    ";",
    ":",
    "'",
    '"',
    ",",
    ".",
    "/",
    "?",
    " ",
]


def exit_program():
    "This is a renamed verion of sys.exit()"
    sys.exit()


def clear_console():
    "Clears the output of the console"
    os.system("CLS")


def exit_clicked(exit_button, print_on_exit=False):
    "exits the program when you click the exit button"
    # try:
    if keyboard.is_pressed(str(exit_button)):
        if print_on_exit == True:
            print("Ending the program")
        sys.exit()

    # except:
    #     print("please enter a valid exit button")
    #     sys.exit()


def if_clicked(exit_button, print_on_exit=False):
    "Returns True when the specified button is clicked"
    try:
        clicked = False
        if keyboard.is_pressed(str(exit_button)):
            if print_on_exit == True:
                print("The button was clicked")
            clicked = True
    except:
        print("please enter a valid input key")
        sys.exit()
    return clicked


def screen_dimentions():
    "Returns the screen dmentions (x,y)"
    screen_width, screen_height = pyautogui.size()
    return screen_width, screen_height


def press_and_hold_(held_key, pressed_key):
    "Holds the first button and presses the second button"
    try:
        keyboard.press(str(held_key))
        time.sleep(0.05)
        keyboard.press(str(pressed_key))
        keyboard.release(str(held_key))
    except:
        print("please enter a valid key to be held and a valid key to be pressed")


def mouse_pos():
    "Returns the mouse position on the screen (x,y)"
    mouse = Controller()
    pos_x, pos_y = mouse.position
    return pos_x, pos_y


def list_to_string(lst):
    "Turns the list that you give it into a string"
    string = "".join(map(str, lst))
    return string


def mouse_click(button_pressed):
    "Presses the specified mouse button"
    mouse = Controller()
    try:
        if str(button_pressed).lower() == "left":
            mouse.click(Button.left, 1)
        elif str(button_pressed).lower() == "right":
            mouse.click(Button.right, 1)
        elif str(button_pressed).lower() == "middle":
            mouse.click(Button.middle, 1)
        else:
            print("please enter a valid mouse button to click")
            sys.exit()
    except:
        print("please enter a valid mouse button to click")
        sys.exit()


def mouse_scroll(y, x=0):
    "scrolls the mouse by the specified amount (y,x)"
    try:
        x = int(x)
        y = int(y)
        mouse = Controller()
        mouse.scroll(x, y)
    except:
        print("please enter valid integer value for x and y")


def counter(max, num_of_digits=-1):
    try:
        if num_of_digits <= 0:
            num_of_digits = len(list(str(max)))
        # print(num_of_digits)
        max += 1
        for i in range(max):
            num_list = []
            num = i
            num_list = [char for char in str(num)]
            if len(num_list) == num_of_digits:
                print(num)
                # pass
            while len(num_list) < num_of_digits:
                num_list = num_list[::-1]  # Reverse the list using slicing
                num_list = list(num_list)
                num_list.append("0")
                num_list = num_list.__reversed__()
                num_list = "".join(num_list)  # Convert list back to string
                if len(num_list) == num_of_digits:
                    print(num_list)
                    # pass
    except:
        print("Please enter a valid number of digits and a valid maximun")


def mouse_click_hold_start(button_pressed, print_off_notice=False):
    "Holds down the specified button, make sure to put the following function after this one"
    mouse = Controller()
    if print_off_notice == True:
        print(
            "make sure to have the function: mouse_click_hold_end :to end the click: mouse_click_hold_end"
        )
    try:
        if str(button_pressed).lower() == "left":
            mouse.press(Button.left)
        elif str(button_pressed).lower() == "right":
            mouse.press(Button.right)
        elif str(button_pressed).lower() == "middle":
            mouse.press(Button.middle)
        else:
            print("please enter a valid mouse button to hold")
            sys.exit()
    except:
        print("please enter a valid mouse button to hold")
        sys.exit()


def mouse_click_hold_end(button_pressed):
    "Stops holding down the specified button (used after the function) mouse_click_hold_start"
    mouse = Controller()
    try:
        if str(button_pressed).lower() == "left":
            mouse.release(Button.left)
        elif str(button_pressed).lower() == "right":
            mouse.release(Button.right)
        elif str(button_pressed).lower() == "middle":
            mouse.release(Button.middle)
        else:
            print("please enter a valid mouse button to stop holding")
            sys.exit()
    except:
        print("please enter a valid mouse button to stop holding")
        sys.exit()


def keyboard_click(key_pressed):
    "Presses the specified keyboard button"
    try:
        keyboard.press(str(key_pressed))
    except:
        print("please enter a valid key to press")
        sys.exit()


def keyboard_click_shift(key_pressed):
    "shift clicks the specified button"
    try:
        keyboard.press("shift")
        keyboard.press(str(key_pressed))
        keyboard.release("shift")
    except:
        print("please enter a valid key to press")
        sys.exit()


def keyboard_type_word(words):
    "types the specified sentance"
    shift_letters_dict = {
        "~": True,
        "!": True,
        "@": True,
        "#": True,
        "$": True,
        "%": True,
        "^": True,
        "&": True,
        "*": True,
        "(": True,
        ")": True,
        "_": True,
        "+": True,
        "Q": True,
        "W": True,
        "E": True,
        "T": True,
        "Y": True,
        "U": True,
        "I": True,
        "O": True,
        "P": True,
        "{": True,
        "}": True,
        "|": True,
        "A": True,
        "S": True,
        "D": True,
        "F": True,
        "G": True,
        "H": True,
        "J": True,
        "K": True,
        "L": True,
        ":": True,
        '"': True,
        "Z": True,
        "X": True,
        "C": True,
        "V": True,
        "B": True,
        "N": True,
        "M": True,
        "<": True,
        ".": True,
        "?": True,
    }
    try:
        letters = list(words)
        # print(letters)
        for letter in letters:
            if shift_letters_dict.get(letter) == True:
                keyboard.press("shift")
                keyboard.press(letter)
                time.sleep(0.005)
                keyboard.release(letter)
                keyboard.release("shift")
            else:
                keyboard.press(letter)
                time.sleep(0.005)
                keyboard.release(letter)
    except:
        print("please enter a valid key to press")
        sys.exit()


def keyboard_hold(key_pressed, length):
    "Holds the specified keyboard button for the specified length"
    try:
        keyboard.press(str(key_pressed))
        time.sleep(float(length))
        keyboard.release(str(key_pressed))
    except:
        print("please enter a valid key to press and a valid time amount")
        sys.exit()


def find_image_WIP():
    try:
        startButton = pyautogui.locateOnScreen("slide_3.png")
        print(startButton)
    except:
        print("None")


def mouse_if_click_WIP(mouse_button):
    mouse = Listener()


def move_mouse(x, y):
    "moves the mouse by the specified amounts (x,y)"
    mouse = Controller()
    try:
        x = int(x)
        y = int(y)
        mouse.move(x, y)
    except:
        print("Please enter a valid x and y amount")
        sys.exit()


def move_mouse_pos(x, y):
    "Moves the mouse to a specified cordinate"
    mouse = Controller()
    try:
        x = int(x)
        y = int(y)
        current_pos_x, current_pos_y = mouse.position
        x_movment_amt = x - current_pos_x
        y_movment_amt = y - current_pos_y
        mouse.move(x_movment_amt, y_movment_amt)
    except:
        print("Please enter a valid (x,y) cordinate")
        sys.exit()


def convert_to_base_WIP(number_input, base, is_weird_base=False):
    numbers_above_11_dict = {
        10: "A",
        11: "B",
        12: "C",
        13: "D",
        14: "E",
        15: "F",
        16: "G",
        17: "H",
        18: "I",
        19: "J",
        20: "K",
        21: "L",
        22: "M",
        23: "N",
        24: "O",
        25: "P",
        26: "Q",
        27: "R",
        28: "S",
        29: "T",
        30: "U",
        31: "V",
        32: "W",
        33: "X",
        34: "Y",
        35: "Z",
    }
    number = number_input
    while True:
        try:
            base = float(base)
            if base <= 1:
                ValueError
            break
        except ValueError:
            print("please enter a valid base")
    base_input_number = []
    while True:
        number_remainder = number % base
        number_next_div = number / base
        # print(number_next_div)
        number_next_div = math.trunc(number_next_div)
        # print(number_next_div)
        # print(number_remainder)
        # print(number_next_div, " div num")
        # use the is_weird_base if it is above base 36 or it is a non int number
        if is_weird_base == True:
            base_input_number.append("(" + str(number_remainder) + ")")
        else:
            if number_remainder > 9:
                base_input_number.append(numbers_above_11_dict.get(number_remainder))
            else:
                base_input_number.append(number_remainder)
        # print(base_input_number)

        if number_next_div < base:
            base_input_number.append(number_next_div)
            break
        number = number_next_div
    base_input_number.reverse()
    str(base_input_number)
    base_input_number_string = "".join(map(str, base_input_number))
    return base_input_number_string
