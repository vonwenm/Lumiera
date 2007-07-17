

#include "plugin.h"
#include "hello_interface.h"


int
main(int argc, char** argv)
{
  /*
    we have a plugin 'hello_1' which provides us 2 hello interfaces, one for english and one for german output,
    open both try them, close them.
  */

  struct hello_interface_1* hello_de =
    cinelerra_interface_open ("hello_1", "german_1", sizeof(struct hello_interface_1));
  if (!hello_de) CINELERRA_DIE;

  hello_de->say_hello();


  struct hello_interface_1* hello_en =
    cinelerra_interface_open ("hello_1", "english_1", sizeof(struct hello_interface_1));
  if (!hello_en) CINELERRA_DIE;

  hello_en->say_hello();

  cinelerra_interface_close (hello_en);
  cinelerra_interface_close (hello_de);

#if 0
  /*
    same again for a plugin written in C++
  */
  hello_de =
    cinelerra_interface_open ("hellocpp_1", "german_1", sizeof(struct hello_interface_1));
  if (!hello_de) CINELERRA_DIE;

  hello_de->say_hello();


  hello_en =
    cinelerra_interface_open ("hellocpp_1", "english_1", sizeof(struct hello_interface_1));
  if (!hello_en) CINELERRA_DIE;

  hello_en->say_hello();

  cinelerra_interface_close (hello_en);
  cinelerra_interface_close (hello_de);
#endif

  return 0;
}