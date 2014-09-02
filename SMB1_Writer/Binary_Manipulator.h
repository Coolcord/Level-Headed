#ifndef BINARY_MANIPULATOR_H
#define BINARY_MANIPULATOR_H

#include <QString>
#include <QByteArray>
#include <QBitArray>

namespace Binary_Manipulator {
    QString Hex_To_Binary_String(int hex);
    QString Hex_Digit_To_Binary_String(int hexDigit);
    QBitArray Hex_To_BitArray(int hex);
    QBitArray Hex_Digit_To_BitArray(int hexDigit);
    int Binary_String_To_Hex(const QString &binary);
    int BitArray_To_Hex(const QBitArray &bits, int start, int end);
    int BitArray_To_Hex(const QBitArray &bits);
    QString BitArray_To_String(const QBitArray &bits);
    QBitArray Combine_BitArrays(const QBitArray &a, const QBitArray &b);
    void Write_Hex_Digit_To_BitArray(QBitArray &bits, int start, int hexDigit);
    void Write_Hex_Digit_To_BitArray(QBitArray &bits, int start, int hexDigit, unsigned int hexStart, unsigned int hexEnd);
    void Write_Byte_To_BitArray(QBitArray &bits, int start, int hex);
}

#endif // BINARY_MANIPULATOR_H
