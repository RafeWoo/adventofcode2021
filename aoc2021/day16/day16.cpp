#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ranges>
#include <string_view>
#include <iomanip>
#include <algorithm>
#include <format>
#include <set>
#include <map>
#include <limits>
#include <array>
#include <stack>
#include <numeric>
#include <assert.h>
#include <unordered_map>
#include <iterator>
#include <bitset>
#include <functional>

import input_lib;

#if 0

struct literal_data 
{
	std::byte version : 3;
	std::byte type_id : 3; //must be 4
	std::byte A : 5;
	std::byte B : 5;
	std::byte C : 5;
	std::byte dummy : 3;
};

struct operator_data
{
	std::byte version : 3;
	std::byte type_id : 3; //must not be 4
	std::byte length_id : 1; // if 0 then next 15 bits are  total length of subpackets else next eleven bits are number of subpackets

};

struct packet
{
	std::byte version : 3;
	std::byte type_id : 3;

	union {
		literal_data

	} data;
};

#endif


size_t read_version(std::string_view version_bits)
{
	size_t version = std::bitset<3>{ std::string{version_bits} }.to_ulong();
	return version;
}

size_t read_type_id(std::string_view type_id_bits)
{
	size_t type_id = std::bitset<3>{ std::string{type_id_bits} }.to_ulong();
	return type_id;
}

std::pair<uint64_t, size_t> read_literal(std::string_view literal_bits)
{
	uint64_t value = 0;
	size_t read_pos = 0;

	bool finished = false;
	do
	{
		finished = literal_bits[read_pos] == '0';
		read_pos += 1;

		size_t nibble = std::bitset<4>{ std::string{literal_bits.substr(read_pos,4)}}.to_ulong();
		read_pos += 4;

		value = value * 16 + nibble;

	}while (!finished);

	return { value, read_pos };
}

bool is_operator_packet_length(std::string_view length_id)
{
	return length_id[0] == '0';
}

size_t read_packet_length(std::string_view length_bits)
{
	return std::bitset<15>{ std::string{ length_bits } }.to_ulong();
}

size_t read_packet_count(std::string_view count_bits)
{
	return std::bitset<11>{ std::string{ count_bits } }.to_ulong();
}



std::pair<std::vector<uint64_t>, size_t> read_packets(std::string_view root, bool is_packet_length, size_t size)
{
	std::vector<uint64_t> packet_values{};

	size_t packet_pos = 0;
	size_t read_pos = 0;
	while (packet_pos < size)
	{
		size_t packet_size = 0;

		//read header 3bits version 
		auto version = read_version(root.substr(read_pos, 3));
		read_pos += 3;
		packet_size += 3;

		//read 3 bitstype id
		auto type_id = read_type_id(root.substr(read_pos, 3));
		read_pos += 3;
		packet_size += 3;

		switch (type_id)
		{

		case 4: //read literal
		{
			auto [literal, literal_size] = read_literal(root.substr(read_pos));
			read_pos += literal_size;
			packet_size += literal_size;

			packet_values.push_back(literal);
		}
		break;

		default: //read operator
		{
			bool op_is_packet_length = is_operator_packet_length(root.substr(read_pos, 1));
			read_pos += 1;
			packet_size += 1;

			size_t count = 0;
			if(op_is_packet_length)
			{
				count = read_packet_length(root.substr(read_pos, 15));
				read_pos += 15;
				packet_size += 15;
			}
			else //packet count
			{
				count = read_packet_count(root.substr(read_pos, 11));
				read_pos += 11;
				packet_size += 11;
			}

			auto const& [sub_values, bits_read] = read_packets(root.substr(read_pos), op_is_packet_length, count );
			read_pos += bits_read;
			packet_size += bits_read;

			uint64_t sub_value = 0;
			switch (type_id)
			{
			case 0: //sum packets
				assert(sub_values.size() >=1);
				sub_value = std::accumulate(sub_values.begin(), sub_values.end(), 0ULL);
				break;

			case 1: //product packets
				assert(sub_values.size() >= 1);
				sub_value = std::accumulate(sub_values.begin(), sub_values.end(), 1ULL, [](auto a, auto b) {return a * b; });
				break;

			case 2: //min 
			{
				auto it = std::ranges::min_element(sub_values);
				assert(it != sub_values.end());
				sub_value = *it;
			}
				break;

			case 3: //max
			{
				auto it = std::ranges::max_element(sub_values);
				assert(it != sub_values.end());
				sub_value = *it;
			}
				break;

			case 5: //gt
				assert( sub_values.size() == 2 );
				sub_value = sub_values[0] > sub_values[1] ? 1 : 0;
				break;

			case 6: //lt
				assert(sub_values.size() == 2);
				sub_value = sub_values[0] < sub_values[1] ? 1 : 0;
				break;

			case 7: //eq
				assert(sub_values.size() == 2);
				sub_value = sub_values[0] == sub_values[1] ? 1 : 0;
				break;

			}

			packet_values.push_back(sub_value);
		}
		break;
		}
		
	

		if (is_packet_length)
		{
			packet_pos += packet_size;
		}
		else
		{
			++packet_pos;
		}
			
	}

	return std::make_pair( std::move(packet_values), read_pos);
	//typeid == 4 -> read literal
	//typeid != 4 -> read operator

	//read_literal 
	// read 1 bit   //1->keep going 0->return
	// read 4 bits 


	//read_operator
	//read length id 0 ->read subpacketlength 1 ->read subpacket total

	//read subpacketlength 
	//read 15 bits -> read packets until length reached

	//read subpackettotal
	//read 11 bits -> read packets until packetcount reached

}

std::byte c_to_byte(char const c)
{
	if ('0' <= c or c <= '9')
		return static_cast<std::byte>(c - '0');
	else
		return static_cast<std::byte>(c - 'A' + 10);
}

uint64_t c_to_cbyte(char const c)
{
	if ('0' <= c and c <= '9')
	{
		return (c - '0');
	}
	else
	{	
		return  c - 'A' + 10;
	}
}
int main(void)
{
	auto input = std::ifstream("../input_files/day16.txt");
	std::string hex_string{};
	std::getline(input, hex_string);

	//std::vector<std::byte> bytes{};
	//std::ranges::transform(hex_string, std::back_inserter(bytes), c_to_byte);

	//hex_string = "620080001611562C8802118E34";
	//hex_string = "38006F45291200";
	//hex_string = "C0015000016115A2E0802F182340";
	//hex_string = "A0016C880162017C3686B18A3D4780";


	std::string binary_string{};
	binary_string.reserve(4 * hex_string.size());
	for (char c : hex_string)
	{
		binary_string += std::bitset<4>{c_to_cbyte(c)}.to_string();
		//std::format_to(std::back_inserter(binary_string), "{:04b}", c_to_cbyte(c) );
	}

	//binary_string = "00111000000000000110111101000101001010010001001000000000";

	

	auto const& [values, bits_read] = read_packets(binary_string, false, 1);

	std::cout << std::format("The packet evaluates to  {}", values[0]);
	return 0;
}