#ifndef UTILS_H
#define UTILS_H
#pragma once

#include <QString>

/**
 * Contains static functions, that can be used by any other class
 */
class Utils
{
public:
    Utils() = delete; //Forbid using the constructor for this class
    static QString getFilmNameFromToken(QString token); //Gets film name from token
};

#endif // UTILS_H
