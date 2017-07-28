// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

ostream& operator<< (ostream& lhs, const FVector& rhs);

ostream& operator<< (ostream& lhs, const FVector2D& rhs);

ostream& operator<< (ostream& lhs, const FRotator& rhs);

ostream& operator<< (ostream& lhs, const FString& rhs);

ostream& operator<< (ostream& lhs, const FName& rhs);