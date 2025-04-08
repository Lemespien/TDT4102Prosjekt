#pragma once
#include "Vector2.h"
#include <assert.h>
#include <iostream>

void Vector2Test() {
    auto A = Vector2(2, 5);
    assert(("Declaration | A x: " && A.x == 2));
    assert(("Declaration | A y: " && A.y == 5));
    auto B = Vector2(-4, 10);
    assert(("Declaration | B x: " && B.x == -4));
    assert(("Declaration | B y: " && B.y == 10));

    auto C = A.directionTo(B);
    assert(("Direction | C x: " && std::round(C.x * 10e5) / 10e5 == -0.768221));
    assert(("Direction | C y: " && std::round(C.y * 10e5) / 10e5 == 0.640184));

    auto D = A / B;
    assert(("Division | D x: " && D.x == -0.5));
    assert(("Division | D y: " && D.y == 0.5));
    
    auto E = A / 2;
    assert(("Division double | E x: " && E.x == 1.0));
    assert(("Division double | E y: " && E.y == 2.5));
    
    auto F = A - B;
    assert(("Subtraction | F x: " && F.x == 6.0));
    assert(("Subtraction | F y: " && F.y == -5.0));

    auto F2 = A - 5;
    assert(("Subtraction double | F2 x: " && F2.x == -3.0));
    assert(("Subtraction double | F2 y: " && F2.y == 0.0));

    auto F3 = 5 - A;
    assert(("Subtraction double | F3 x: " && F3.x == 3.0));
    assert(("Subtraction double | F3 y: " && F3.y == 0.0));
    
    auto G = A + B;
    assert(("Addition | G x: " && G.x == -2.0));
    assert(("Addition | G y: " && G.y == 15.0));
    
    auto G2 = A + 5;
    assert(("Addition double | G2 x: " && G2.x == 7.0));
    assert(("Addition double | G2 y: " && G2.y == 10.0));

    auto G3 = 5 + A;
    assert(("Addition double | G3 x: " && G3.x == 7.0));
    assert(("Addition double | G3 y: " && G3.y == 10.0));

    auto H = A * B;
    assert(("Multiplication vector2 | H x: " && H.x == -8.0));
    assert(("Multiplication vector2 | H y: " && H.y == 50.0));

    auto H2 = A * 5.0;
    assert(("Multiplication double | H2 x: " && H2.x == 10.0));
    assert(("Multiplication double | H2 y: " && H2.y == 25.0));
    
    auto H3 = 5.0 * A;
    assert(("Multiplication double | H3 x: " && H3.x == 10.0));
    assert(("Multiplication double | H3 y: " && H3.y == 25.0));

    assert(("Unmuted | A x: " && A.x == 2));
    assert(("Unmuted | A y: " && A.y == 5));

    assert(("Unmuted | B x: " && B.x == -4));
    assert(("Unmuted | B y: " && B.y == 10));

    Vector2 test = Vector2(2, 5);
    Vector2 test2 = Vector2(2, 5);

    test += test2;
    assert(("addition += | test x: " && test.x == 4));
    assert(("addition += | test y: " && test.y == 10));
    test2 = test + test2;
    assert(("addition + | test2 x: " && test2.x == 6));
    assert(("addition + | test2 y: " && test2.y == 15));
    test += test2;
    assert(("addition += | test x: " && test.x == 10));
    assert(("addition += | test y: " && test.y == 25));


    std::cout << "Vector2 | All tests passed!" << std::endl;

}
