/* The null stream will silently drop anything you throw to it.

  Internally it's just an ostream with the default constructor.

  You don't need to instantiate this class, just use the static cnull
  the same way you use cout or cerr. */

#ifndef NULLSTREAM_H
#define NULLSTREAM_H

#include <ostream>

class NullStream : public std::ostream {
public:
   static const NullStream cnull;
};
NullStream const NullStream::cnull;

#endif /* NULLSTREAM_H */
