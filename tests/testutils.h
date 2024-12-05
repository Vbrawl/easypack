#ifndef EASYPACK_TESTUTILS_H
#define EASYPACK_TESTUTILS_H

#define RUN_TEST(status, shifter, incrementor, func) do {\
  status |= (func() << shifter);\
  shifter += incrementor;\
} while(0)



#endif
