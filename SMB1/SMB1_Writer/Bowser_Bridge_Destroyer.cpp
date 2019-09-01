#include "Bowser_Bridge_Destroyer.h"
#include "../../Common_Files/Random.h"
#include <assert.h>
#include <QVector>

bool Bowser_Bridge_Destroyer::Alternating() {
    switch (Random::Get_Instance().Get_Num(0, 3)) {
    default:    assert(false); return false;
    case 0:     return this->Write_Bytes_To_Offset(0x4FEF, QByteArray::fromHex(QString("8084888C90949882868A8E9296").toLatin1())); //Backwards then Backwards
    case 1:     return this->Write_Bytes_To_Offset(0x4FEF, QByteArray::fromHex(QString("8084888C90949896928E8A8682").toLatin1())); //Backwards then Forwards
    case 2:     return this->Write_Bytes_To_Offset(0x4FEF, QByteArray::fromHex(QString("9894908C88848082868A8E9296").toLatin1())); //Forwards then Backwards
    case 3:     return this->Write_Bytes_To_Offset(0x4FEF, QByteArray::fromHex(QString("9894908C88848096928E8A8682").toLatin1())); //Forwards then Forwards
    }
}

bool Bowser_Bridge_Destroyer::Forwards() {
    return this->Write_Bytes_To_Offset(0x4FEF, QByteArray::fromHex(QString("98969492908E8C8A8886848280").toLatin1()));
}

bool Bowser_Bridge_Destroyer::Backwards() {
    return this->Write_Bytes_To_Offset(0x4FEF, QByteArray::fromHex(QString("80828486888A8C8E9092949698").toLatin1()));
}

bool Bowser_Bridge_Destroyer::Chaotic() {
    QByteArray bridgeBytes = QByteArray::fromHex(QString("98969492908E8C8A8886848280").toLatin1());
    QByteArray bytes(13, static_cast<char>(0x00));
    assert(bridgeBytes.size() == 13);
    int maxValue = bridgeBytes.size();
    for (int i = 0; i < maxValue; ++i) {
        int index = Random::Get_Instance().Get_Num(0, bridgeBytes.size()-1);
        bytes.data()[i] = bridgeBytes.at(index);
        bridgeBytes.remove(index, 1);
    }
    return this->Write_Bytes_To_Offset(0x4FEF, bytes);
}

bool Bowser_Bridge_Destroyer::From_Center() {
    return this->Write_Bytes_To_Offset(0x4FEF, QByteArray::fromHex(QString("8C8E8A90889286948496829880").toLatin1()));
}

bool Bowser_Bridge_Destroyer::To_Center() {
    return this->Write_Bytes_To_Offset(0x4FEF, QByteArray::fromHex(QString("809882968494869288908A8E8C").toLatin1()));
}

bool Bowser_Bridge_Destroyer::Grouped() {
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return false;
    case 0:     return this->Groups_Of_3();
    case 1:     return this->Groups_Of_4();
    case 2:     return this->Groups_Of_6();
    }
}

bool Bowser_Bridge_Destroyer::Groups_Of_3() {
    QByteArray group1, group2, group3, group4;
    QByteArray bytes;
    if (Random::Get_Instance().Get_Num(1)) group1 = QByteArray::fromHex(QString("989694").toLatin1()); //forwards
    else group1 = QByteArray::fromHex(QString("949698").toLatin1()); //backwards
    if (Random::Get_Instance().Get_Num(1)) group2 = QByteArray::fromHex(QString("92908E").toLatin1()); //forwards
    else group2 = QByteArray::fromHex(QString("8E9092").toLatin1()); //backwards
    if (Random::Get_Instance().Get_Num(1)) group3 = QByteArray::fromHex(QString("8C8A88").toLatin1()); //forwards
    else group3 = QByteArray::fromHex(QString("888A8C").toLatin1()); //backwards
    if (Random::Get_Instance().Get_Num(1)) group4 = QByteArray::fromHex(QString("86848280").toLatin1()); //forwards
    else group4 = QByteArray::fromHex(QString("80828486").toLatin1()); //backwards

    //Build the Bytes Array
    assert(group1.size() == group2.size());
    assert(group2.size() == group3.size());
    assert(group3.size()+1 == group4.size());
    if (Random::Get_Instance().Get_Num(1)) { //run the groups at the same time
        bytes = QByteArray(13, static_cast<char>(0x00));
        for (int i = 0, j = 0; i < group1.size(); ++i, j += 4) {
            bytes.data()[j] = group4.at(i);
            bytes.data()[j+1] = group3.at(i);
            bytes.data()[j+2] = group2.at(i);
            bytes.data()[j+3] = group1.at(i);
        }
        bytes.data()[12] = group4.at(3);
    } else { //run the groups one at a time
        QVector<int> groups(4);
        for (int i = 0; i < groups.size(); ++i) groups.data()[i] = i+1;
        while (!groups.isEmpty()) {
            int index = Random::Get_Instance().Get_Num(groups.size()-1);
            switch (groups.at(index)) {
            default:    assert(false); return false;
            case 1:     bytes += group1; break;
            case 2:     bytes += group2; break;
            case 3:     bytes += group3; break;
            case 4:     bytes += group4; break;
            }
            groups.remove(index);
        }
    }
    return this->Write_Bytes_To_Offset(0x4FEF, bytes);
}

bool Bowser_Bridge_Destroyer::Groups_Of_4() {
    QByteArray group1, group2, group3;
    QByteArray bytes;
    if (Random::Get_Instance().Get_Num(1)) group1 = QByteArray::fromHex(QString("98969492").toLatin1()); //forwards
    else group1 = QByteArray::fromHex(QString("92949698").toLatin1()); //backwards
    if (Random::Get_Instance().Get_Num(1)) group2 = QByteArray::fromHex(QString("908E8C8A").toLatin1()); //forwards
    else group2 = QByteArray::fromHex(QString("8A8C8E90").toLatin1()); //backwards
    if (Random::Get_Instance().Get_Num(1)) group3 = QByteArray::fromHex(QString("8886848280").toLatin1()); //forwards
    else group3 = QByteArray::fromHex(QString("8082848688").toLatin1()); //backwards

    //Build the Bytes Array
    assert(group3.size() == group1.size()+1);
    assert(group1.size() == group2.size());
    if (Random::Get_Instance().Get_Num(1)) { //run the groups at the same time
        bytes = QByteArray(13, static_cast<char>(0x00));
        for (int i = 0, j = 0; i < group1.size(); ++i, j += 3) {
            bytes.data()[j] = group3.at(i);
            bytes.data()[j+1] = group2.at(i);
            bytes.data()[j+2] = group1.at(i);
        }
        bytes.data()[12] = group3.at(4);
    } else { //run the groups one at a time
        QVector<int> groups(3);
        for (int i = 0; i < groups.size(); ++i) groups.data()[i] = i+1;
        while (!groups.isEmpty()) {
            int index = Random::Get_Instance().Get_Num(groups.size()-1);
            int group = groups.at(index);
            if (group == 1) bytes += group1;
            else if (group == 2) bytes += group2;
            else if (group == 3) bytes += group3;
            else assert(false);
            groups.remove(index);
        }
    }
    return this->Write_Bytes_To_Offset(0x4FEF, bytes);
}

bool Bowser_Bridge_Destroyer::Groups_Of_6() {
    //Select Forwards and Backwards at Random
    QByteArray group1, group2;
    QByteArray bytes(13, static_cast<char>(0x00));
    if (Random::Get_Instance().Get_Num(1)) group1 = QByteArray::fromHex(QString("98969492908E").toLatin1()); //forwards
    else group1 = QByteArray::fromHex(QString("8E9092949698").toLatin1()); //backwards
    if (Random::Get_Instance().Get_Num(1)) group2 = QByteArray::fromHex(QString("8C8A8886848280").toLatin1()); //forwards
    else group2 = QByteArray::fromHex(QString("80828486888A8C").toLatin1()); //backwards

    //Build the Bytes Array
    assert(group2.size() == group1.size()+1);
    for (int i = 0, j = 0; i < group1.size(); ++i, j += 2) {
        bytes.data()[j] = group2.at(i);
        bytes.data()[j+1] = group1.at(i);
    }
    bytes.data()[12] = group2.at(6);
    return this->Write_Bytes_To_Offset(0x4FEF, bytes);
}

