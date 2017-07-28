// Fill out your copyright notice in the Description page of Project Settings.

#include "Extensions.h"

ostream& operator<< (ostream& lhs, const FVector& rhs) {
	stringstream ss;
	ss << "(" << rhs.X << "; " << rhs.Y << "; " << rhs.Z << ")";
	lhs << ss.str();
	return lhs;
}

ostream& operator<< (ostream& lhs, const FVector2D& rhs) {
	stringstream ss;
	ss << "(" << rhs.X << "; " << rhs.Y << ")";
	lhs << ss.str();
	return lhs;
}

ostream& operator<< (ostream& lhs, const FRotator& rhs) {
	stringstream ss;
	ss << "(" << rhs.Pitch << "; " << rhs.Yaw << "; " << rhs.Roll << ")";
	lhs << ss.str();
	return lhs;
}

ostream& operator<< (ostream& lhs, const FString& rhs) {
	lhs << string(TCHAR_TO_UTF8(*rhs));
	return lhs;
}

ostream& operator<< (ostream& lhs, const FName& rhs) {
	lhs << string(TCHAR_TO_UTF8(rhs.GetPlainANSIString()));
	return lhs;
}
