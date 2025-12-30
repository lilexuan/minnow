#include <algorithm>
#include <cstdint>

#include "byte_stream.hh"
#include "debug.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

// Push data to stream, but only as much as available capacity allows.
void Writer::push( string data )
{
  if ( data.length() + buffer.length() > capacity_ ) {
    data = data.substr( 0, capacity_ - buffer.length() );
  }
  buffer += data;
  bytes_pushed_ += data.length();
}

// Signal that the stream has reached its ending. Nothing more will be written.
void Writer::close()
{
  is_closed_ = true;
}

// Has the stream been closed?
bool Writer::is_closed() const
{
  return is_closed_;
}

// How many bytes can be pushed to the stream right now?
uint64_t Writer::available_capacity() const
{
  return capacity_ - buffer.length();
}

// Total number of bytes cumulatively pushed to the stream
uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_;
}

// Peek at the next bytes in the buffer -- ideally as many as possible.
// It's not required to return a string_view of the *whole* buffer, but
// if the peeked string_view is only one byte at a time, it will probably force
// the caller to do a lot of extra work.
string_view Reader::peek() const
{
  return buffer;
}

// Remove `len` bytes from the buffer.
void Reader::pop( uint64_t len )
{
  uint64_t pop_len = min( len, static_cast<uint64_t>( buffer.length() ) );
  buffer = buffer.substr( pop_len );
  bytes_popped_ += pop_len;
}

// Is the stream finished (closed and fully popped)?
bool Reader::is_finished() const
{
  return is_closed_ && buffer.empty();
}

// Number of bytes currently buffered (pushed and not popped)
uint64_t Reader::bytes_buffered() const
{
  return buffer.length();
}

// Total number of bytes cumulatively popped from stream
uint64_t Reader::bytes_popped() const
{
  return bytes_popped_;
}
