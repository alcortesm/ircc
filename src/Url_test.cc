#include <cassert>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <cstdlib>

#include "Url.h"
#include "DebugStream.h"
extern DebugStream debug;

using std::endl;

/* static */ void
Url::Test(void)
{
   debug << "*** Begin of Url test" << endl ;
   // test the private constructor
   // this test involves a DNS resolution, you'll need a working resolver
   // of it.uc3m.es
   {
      const char * proto = "http";
      const char * domain = "it.uc3m.es";
      const char * ip = "163.117.139.31";
      uint16_t port = 8080;
      const char * path = "alcortes/index.html";
      const char * query = "test.php?other=yes";
      const char * anchor = "bla";
      const char * canonical = "http://it.uc3m.es:8080/alcortes/index.html?test.php?other=yes#bla";

      try {
         Url url = Url(proto, port, domain, path, query, anchor);
         assert(url.Proto() == proto);
         assert(url.Port() == port);
         assert(url.PortNbo() == htons(port));
         assert(url.Domain() == domain);
         struct in_addr ia;
         {
            int r = inet_aton(ip, &ia);
            if (r == 0) {
               std::cerr << "Provided an invalid ip: %s\n" << endl;
               goto error;
            }
         }
         assert(memcmp(&url.Addr(), &ia, sizeof(ia)) == 0);
         assert(url.Ip() == "163.117.139.31");
         assert(url.Path() == path);
         assert(url.Query() == query);
         assert(url.Anchor() == anchor);
         assert(url.Canonical() == canonical);
         assert(url.HasPath() == true);
         assert(url.HasQuery() == true);
         assert(url.HasAnchor() == true);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      } catch (Url::NameResolutionException & nre) {
         std::cerr << nre.what() << endl;
         goto error;
      } catch (Url::NetworkException & ne) {
         std::cerr << ne.what() << endl;
         goto error;
      }
   }
   // more tests on the private constructor
   // no more DNS resolutions from now on
   // no anchor
   {
      const char * proto = "http";
      const char * domain = "it.uc3m.es";
      uint16_t port = 8080;
      const char * path = "alcortes/index.html";
      const char * query = "test.php?other=yes";
      const char * anchor = "";
      const char * canonical = "http://it.uc3m.es:8080/alcortes/index.html?test.php?other=yes";

      try {
         Url url = Url(proto, port, domain, path, query, anchor);
         assert(url.Proto() == proto);
         assert(url.Port() == port);
         assert(url.PortNbo() == htons(port));
         assert(url.Domain() == domain);
         assert(url.Path() == path);
         assert(url.Query() == query);
         assert(url.Anchor() == anchor);
         assert(url.Canonical() == canonical);
         assert(url.HasPath() == true);
         assert(url.HasQuery() == true);
         assert(url.HasAnchor() == false);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
   }
   // no query
   {
      const char * proto = "http";
      const char * domain = "it.uc3m.es";
      uint16_t port = 8080;
      const char * path = "alcortes/index.html";
      const char * query = "";
      const char * anchor = "bla";
      const char * canonical = "http://it.uc3m.es:8080/alcortes/index.html#bla";

      try {
         Url url = Url(proto, port, domain, path, query, anchor);
         assert(url.Proto() == proto);
         assert(url.Port() == port);
         assert(url.PortNbo() == htons(port));
         assert(url.Domain() == domain);
         assert(url.Path() == path);
         assert(url.Query() == query);
         assert(url.Anchor() == anchor);
         assert(url.Canonical() == canonical);
         assert(url.HasPath() == true);
         assert(url.HasQuery() == false);
         assert(url.HasAnchor() == true);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
   }
   // with separators
   {
      const char * proto = "http";
      const char * domain = "it.uc3m.es";
      uint16_t port = 8080;
      const char * path = "/alcortes/index.html";
      const char * query = "?name=yes?user=no";
      const char * anchor = "#bla";
      const char * canonical = "http://it.uc3m.es:8080/alcortes/index.html?name=yes?user=no#bla";

      try {
         Url url = Url(proto, port, domain, path, query, anchor);
         assert(url.Proto() == proto);
         assert(url.Port() == port);
         assert(url.PortNbo() == htons(port));
         assert(url.Domain() == domain);
         assert(url.Path() == path);
         assert(url.Query() == query);
         assert(url.Anchor() == anchor);
         assert(url.Canonical() == canonical);
         assert(url.HasPath() == true);
         assert(url.HasQuery() == true);
         assert(url.HasAnchor() == true);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
   }
   {
      const char * url_txt = "hTTp://gwc.iblInx.coM:2108/gwc/cgi-BIn/fc";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "gwc.iblinx.com");
      assert(p_url->Port()  == 2108);
      assert(p_url->Path()  == "/gwc/cgi-bin/fc");
      delete(p_url);
   }
   {
      const char * url_txt = "hTTp://gwc.iblinx.net:13/fc?bla.php#foobar";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "gwc.iblinx.net");
      assert(p_url->Port()  == 13);
      assert(p_url->Path()  == "/fc");
      assert(p_url->Query()  == "bla.php");
      assert(p_url->Anchor()  == "foobar");
      delete p_url;
   }
   {
      const char * url_txt = "http://gwc.iblinx.com/";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "gwc.iblinx.com");
      assert(p_url->Port()  == 80);
      assert(p_url->Path()  == "");
      delete p_url;
   }
   {
      const char * url_txt = "hTTp://gwc.iblInx.coM:2108";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "gwc.iblinx.com");
      assert(p_url->Port()  == 2108);
      assert(p_url->Path()  == "");
      delete p_url;
   }
   {
      const char * url_txt = "hTTp://gwc.iblInx.coM:2108#bla";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "gwc.iblinx.com");
      assert(p_url->Port()  == 2108);
      assert(p_url->Path()  == "");
      assert(p_url->Anchor() == "bla");
      delete p_url;
   }
   {
      const char * url_txt = "hTTp://a";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "a");
      assert(p_url->Port()  == 80);
      assert(p_url->Path()  == "");
      delete p_url;
   }
   {
      const char * url_txt = "hTTp://a/bla/foo.sh";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "a");
      assert(p_url->Port()  == 80);
      assert(p_url->Path()  == "/bla/foo.sh");
      delete p_url;
   }
   {
      const char * url_txt = "hTTp://a:1234";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "a");
      assert(p_url->Port()  == 1234);
      assert(p_url->Path()  == "");
      delete p_url;
   }
   {
      const char * url_txt = "htatp://www.google.com:120/bla/foo.sh";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         goto ok1;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      std::cerr << "An exception was not thrown (bad proto)" << endl;
      exit(EXIT_FAILURE);
   ok1:;
   }
   {
      const char * url_txt = "ahttp://www.google.com:120/bla/foo.sh";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         goto ok2;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      std::cerr << "An exception was not thrown (prefix at proto)" << endl;
      exit(EXIT_FAILURE);
   ok2:;
   }
   {
      const char * url_txt = "http:/a/www.google.com:120/bla/foo.sh";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         goto ok3;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      std::cerr << "An exception was not thrown (bad colon_slash_slash)" << endl;
      exit(EXIT_FAILURE);
   ok3:;
   }
   {
      const char * url_txt = "http://:120/bla/foo.sh";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         goto ok4;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      std::cerr << "An exception was not thrown (no domain name with port)" << endl;
      exit(EXIT_FAILURE);
   ok4:;
   }
   {
      const char * url_txt = "http://www.google.com:12a0/bla/foo.sh";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         goto ok5;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      std::cerr << "An exception was not thrown (bad port)" << endl;
      exit(EXIT_FAILURE);
   ok5:;
      {
         const char * url_txt = "http:///";
         Url * p_url;
         try {
            p_url = new Url(url_txt);
         } catch (Url::MalformedUrlException & mue) {
            goto ok6;
         } catch (std::bad_alloc & ba) {
            std::cerr << ba.what() << ": out of memory" << endl;
            goto error;
         }
         std::cerr << "An exception was not thrown (no domain, no port, no path)" << endl;
         exit(EXIT_FAILURE);
      ok6:;
      }
      {
         const char * url_txt = "http://:32";
         Url * p_url;
         try {
            p_url = new Url(url_txt);
         } catch (Url::MalformedUrlException & mue) {
            goto ok7;
         } catch (std::bad_alloc & ba) {
            std::cerr << ba.what() << ": out of memory" << endl;
            goto error;
         }
         std::cerr << "An exception was not thrown (no domain, no path)" << endl;
         exit(EXIT_FAILURE);
      ok7:;
      }
      {
         const char * url_txt = "http://www.google.com:-32/bla/foo.sh";
         Url * p_url;
         try {
            p_url = new Url(url_txt);
         } catch (Url::MalformedUrlException & mue) {
            goto ok8;
         } catch (std::bad_alloc & ba) {
            std::cerr << ba.what() << ": out of memory" << endl;
            goto error;
         }
         std::cerr << "An exception was not thrown (negative port number)" << endl;
         exit(EXIT_FAILURE);
      ok8:;
      }
      {
         const char * url_txt = "http://www.google.com:999999/bla/foo.sh";
         Url * p_url;
         try {
            p_url = new Url(url_txt);
         } catch (Url::MalformedUrlException & mue) {
            goto ok9;
         } catch (std::bad_alloc & ba) {
            std::cerr << ba.what() << ": out of memory" << endl;
            goto error;
         }
         std::cerr << "An exception was not thrown (port number too big)" << endl;
         exit(EXIT_FAILURE);
      ok9:;
      }
   }
   debug << "*** End of Url test" << endl ;
   return;
 error:
   exit(EXIT_FAILURE);
}
