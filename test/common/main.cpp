/*
** EPITECH PROJECT, 2021
** main.cpp
** File description:
** main
*/

#include "portaudio.h"
#include "Audio.hpp"
#include <iostream>

int main(int ac, char **av)
{
    Audio a;

    a.record();
    a.play();
    return (0);
}
