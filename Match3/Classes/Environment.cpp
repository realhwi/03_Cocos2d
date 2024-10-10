#include "stdafx.h" 
#include "Environment.h" 

// Global 클래스의 싱글톤 인스턴스 포인터 초기화
Global* instance = nullptr;

// Global 클래스의 싱글톤 인스턴스를 반환하는 함수
Global* Global::getInstance()
{
    // 인스턴스가 없으면 새로운 객체 생성
    if (instance == nullptr) instance = new Global();
    return instance; // 인스턴스 반환
}

// 점수를 설정하는 함수
void Global::setScore(long long value)
{
    this->score = value; // 점수를 전달받은 값으로 설정
}

// 현재 점수를 반환하는 함수
long long Global::getScore()
{
    return score; // 현재 점수 반환
}

// 점수를 더하는 함수
void Global::addScore(long long value)
{
    this->score += value; // 전달받은 값을 현재 점수에 더함
}
