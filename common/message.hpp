#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

#pragma warning(disable : 4996)

class message {
public:
	enum { header_length = 4 };
	enum { type_length = 1 };
	enum { max_body_length = 10 };
	enum { crc16_length = 2 };

	message()
		: body_length_(0) {
		// hack
		data_[header_length + type_length + max_body_length + crc16_length] = '\0';
	}

	const char* data() const {

		return data_;
	}

	char* data() {

		return data_;
	}

	size_t length() const {

		return header_length + type_length + body_length_ + crc16_length;
	}

	const char* body() const {

		return data_ + header_length + type_length;
	}

	char* body() {

		return data_ + header_length + type_length;
	}

	size_t body_length() const {

		return body_length_;
	}

	void body_length(size_t new_length) {

		body_length_ = new_length;
		if (body_length_ > max_body_length)
			body_length_ = max_body_length;
		encode_header();
	}

	unsigned char get_type() {
		decode_type();
		return type_;
	}

	bool set_type(unsigned char new_type) {
		
		if (new_type >= 0 && new_type < 10) {
			type_ = new_type;
			encode_type();
			return true;
		} else
			return false;
	}

	void decode_type() {
		char type[type_length + 1] = "";

		strncat(type, data_ + header_length, type_length);
		type_ = static_cast<unsigned char>(atoi(type));
	}

	void encode_type() {
		char type[type_length + 1] = "";

		sprintf(type, "%1d", static_cast<unsigned char>(type_));
		memcpy(data_ + header_length, type, type_length);
	}

	bool decode_header() {
		char header[header_length + 1] = "";
		strncat(header, data_, header_length);
		body_length_ = atoi(header);
		if (body_length_ > max_body_length) {

			body_length_ = 0;
			return false;
		}
		return true;
	}

	void encode_header() {
		char header[header_length + 1] = "";

		sprintf(header, "%4d", static_cast<int>(type_length + body_length_ + crc16_length));
		memcpy(data_, header, header_length);
	}

	bool copy(const char* message, size_t new_length) {
		bool ok = true;
		body_length_ = new_length;
		if (body_length_ > max_body_length) {
			body_length_ = max_body_length;
			ok = false;
		}
		memcpy(data_ + header_length + type_length, message, body_length_);
		encode_header();
		return ok;
	}

private:
	char data_[header_length + type_length + max_body_length + crc16_length];
	size_t body_length_;
	unsigned char type_;
};

#endif // MESSAGE_HPP