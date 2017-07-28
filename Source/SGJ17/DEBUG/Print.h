#pragma once
#include "CoreMinimal.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

static const fstream LogFile = fstream("log.txt", fstream::out | fstream::trunc);

template<class DT>
void PrintHelper(stringstream &buff, DT arg);

template<class DT, class ...T>
void PrintHelper(stringstream &buff, DT arg, T ...args) {
	buff << arg;
	PrintHelper(buff, args...);
}

template<class DT>
void PrintHelper(stringstream &buff, DT arg) {
	buff << arg;
}

template<class ...T>
void WPrint(T ...args) {
	stringstream buff;
	PrintHelper(buff, args...);
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(buff.str().c_str()));
}

template<class ...T>
void YPrint(T ...args) {
	stringstream buff;
	PrintHelper(buff, args...);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(buff.str().c_str()));
}

template<class ...T>
void RPrint(T ...args) {
	stringstream buff;
	PrintHelper(buff, args...);
	UE_LOG(LogTemp, Error, TEXT("%s"), *FString(buff.str().c_str()));
}