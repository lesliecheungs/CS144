#include "byte_stream.hh"

#include <algorithm>
#include <iterator>
#include <stdexcept>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

using namespace std;

ByteStream::ByteStream(const size_t capacity):_capacity(capacity), _cnt(0), _cur_index(0), _read_cnt(0), _write_cnt(0), _endinput(false)
, _unwritten(true), _buf(vector<char>(capacity)) {}

size_t ByteStream::write(const string &data) {
    if(_endinput)
        return 0;
    
    _unwritten = false;
    int write_num = min(data.size(), _capacity - _cnt);
    int write_index = (_cur_index+_cnt)%_capacity;

    for(int i = 0; i < write_num; i++) {
        _buf[write_index]  = data[i];
        write_index = (write_index+1)%_capacity;
    }

    _cnt += write_num;
    _write_cnt += write_num;
    return write_num;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    int num = min(len, _cnt);

    string str(num, '0');
    for(int i = 0; i < num; i++) {
        str[i] = _buf[(i+_cur_index)%_capacity];
    }

    return str;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    int num = min(len, _cnt);

    _cur_index = (_cur_index+num)%_capacity;
    _cnt -= num;

    _read_cnt += len;
}

void ByteStream::end_input() {
    _endinput = true;
}

bool ByteStream::input_ended() const { return _endinput; }

size_t ByteStream::buffer_size() const { return _cnt; }

bool ByteStream::buffer_empty() const { return _cnt == 0; }

bool ByteStream::eof() const {
    return input_ended() && buffer_empty();
}

size_t ByteStream::bytes_written() const { return _write_cnt; }

size_t ByteStream::bytes_read() const { return _read_cnt; }

size_t ByteStream::remaining_capacity() const { return _capacity - _cnt; }
