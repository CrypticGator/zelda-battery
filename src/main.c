/****
Copyright 2014 Alexej Magura

This file is a part of Zbat

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
****/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header/main.h"
#if _ZB_MAKING_COLOR
#include "header/color.h"
#else
#include "header/text.h"
#endif
#include "header/power.h"

// I may make the linux and bsd sources (more or less) compatible
// since `sysctl' is also present in Linux
// and the `sysctl' code is smaller (code-wise, need to look at asm) than
// the raw `acpi' lib code



#if _ZB_MAKING_COLOR
inline struct color_disp_options_t
#else
inline struct txt_disp_options_t
#endif
opt_parse(int argc, char **argv)
{
  int chara = 0; // character storage for getopt

#if _ZB_MAKING_COLOR
  struct color_disp_options_t color_opts;
  color_opts.acblink = false;
  color_opts.blink = true;
  color_opts.blink_threshold = 3;
  const char *shopts = "hvHanb:";
#else
  struct txt_disp_options_t txt_opts;
  const char *shopts = "hvf:e:";
#if _ZB_UNIX_BSD
  txt_opts.full_heart = "+";
  txt_opts.empty_heart = "-";
#else
  txt_opts.full_heart = "\u2665";
  txt_opts.empty_heart = "\u2661";
#endif
#endif

#if _ZB_UNIX_BSD
  int *opt_counter = 0;

  struct option lopts[] = {
    { 0, 0, 0, 0 }
  };
  
  while ((chara = getopt_long(argc, argv, shopts, lopts, opt_counter)) != EOF) {
#else
#if !_ZB_UNIX_LINUX
#include <unistd.h>
#endif
  while ((chara = getopt(argc, argv, shopts)) != EOF) {
#endif
    _ZB_DEBUG("optopt %d:", optopt);
    if (optopt != 0)
      exit(EXIT_FAILURE);

    switch(chara) {
      case 'h':
        _ZB_MSG("Usage: %s [OPTION]...\n", _ZB_PROGNAME);
#if _ZB_MAKING_COLOR
        _ZB_ARGMSG("-h\tprint this message and exit");
        _ZB_ARGMSG("-v\tprint program version and exit");
#else
        _ZB_ARGMSG("-h\t\tprint this message and exit");
        _ZB_ARGMSG("-v\t\tprint program version and exit");
#endif
        _ZB_DISP_HELP();
        exit(EXIT_FAILURE);
      case 'v':
        _ZB_MSG("%s", PACKAGE_VERSION);
        exit(EXIT_FAILURE);
#if _ZB_MAKING_COLOR
      case 'b':
        _ZB_STRTONUM(color_opts.blink_threshold, (const char *)optarg);
        break;
      case 'a':
        color_opts.acblink = true;
        break;
      case 'n':
        color_opts.blink = false;
        break;
#else
      case 'e':
        txt_opts.empty_heart = optarg;
        break;
      case 'f':
        txt_opts.full_heart = optarg;
        break;
#endif
    }
  }

int
main(int argc, char **argv)
{
  int chara = 0; // character storage


  struct power_t power;
  _ZB_INIT();

#if _ZB_MAKING_COLOR
  _ZB_DISP_PWR_INFO(color_opts);
#else
  _ZB_DISP_PWR_INFO(txt_opts);
#endif
  return EXIT_SUCCESS;
}
