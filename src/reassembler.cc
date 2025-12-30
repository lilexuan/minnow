#include "reassembler.hh"
#include "debug.hh"
#include <cstdint>

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  debug( "unimplemented insert({}, {}, {}) called", first_index, data, is_last_substring );
  // 超过容量直接不插入
  if (data.size() > output_.writer().available_capacity()) {
    return;
  }


}

// How many bytes are stored in the Reassembler itself?
// This function is for testing only; don't add extra state to support it.
uint64_t Reassembler::count_bytes_pending() const
{
  debug( "unimplemented count_bytes_pending() called" );
  return {};
}

void Slice::merge(std::string&& other_data, const uint64_t other_first_index, const uint64_t other_last_index)
{
  // 1. 不重叠
  if ( other_last_index < first_index_ || other_first_index > last_index_ ) {
    return;
  }

  // 2. other 完全包含 this
  if ( other_first_index <= first_index_ && other_last_index >= last_index_ ) {
    data_ = std::move(other_data);
    first_index_ = other_first_index;
    last_index_ = other_last_index;
    return;
  }
  
  // 3. this 完全包含 other
  if ( other_first_index >= first_index_ && other_last_index <= last_index_) {
    return;
  }

  // 4. this在前, other在后, 部分重叠
  if ( other_first_index <= last_index_ ) {
    data_.append(other_data.substr(last_index_ - other_first_index));
    last_index_ = other_last_index;
    return;
  }

  // 5. this在后, other在前, 部分重叠
  if ( other_last_index >= first_index_ ) {
    other_data.append(data_.substr(other_last_index - first_index_));
    data_ = std::move(other_data);
    first_index_ = other_first_index;
    return;
  }
}