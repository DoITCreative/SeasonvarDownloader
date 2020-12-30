#include "../headers/utils.h"

QString Utils::getFilmNameFromToken(QString token)
{
    QString output = "";
    output = token.right(token.length()-token.indexOf("/7f_")-4);
    output = output.left(output.lastIndexOf(".a1"));
    return output;
}
