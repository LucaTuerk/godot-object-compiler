#pragma once

enum TestEnum {
  VALUE_A = 1,
  VALUE_B = 0x1 + 1,
  VALUE_C = 0b10 * 2,
  VALUE_D = 16 / 2,
  VALUE_E = 36 % 20,
  VALUE_F = 32 & 0b100000,
  VALUE_G = 64 | 0,
  VALUE_H = 128 ^ 0,
  VALUE_I = 0x80 << 1,
  VALUE_J = 1024 >> 1,
};

enum TestEnum2 {
  VALUE_A2 = 1,
  VALUE_B2 = VALUE_A2 << VALUE_A2,
  VALUE_C2 = VALUE_B2 << 1,
  VALUE_D2 = VALUE_B2 << VALUE_B2,
  VALUE_E2 = VALUE_D2 << 1,
  VALUE_F2 = VALUE_E2 << 1,
  VALUE_ALL = VALUE_A2 | 0x2 | VALUE_C2 | VALUE_D2 | VALUE_E2 | VALUE_F2,
};

enum TestEnum3 {
  VALUE_A3,
  VALUE_B3,
  VALUE_C3,
  VALUE_D3 = 10,
  VALUE_E3,
  VALUE_F3,
};