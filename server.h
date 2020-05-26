//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

//std::string make_daytime_string1()
//{
//    using namespace std; // For time_t, time and ctime;
//    time_t now = time(0);
//    return ctime(&now);
//}
class tcp_connection : 
	public boost::enable_shared_from_this<tcp_connection> {
	std::string make_daytime_string() {
		std::time_t now = time(0);
		return std::ctime(&now);
<<<<<<< HEAD

=======
>>>>>>> 5ed56816e4d6ec2c71b8f730cbe378d16c578af3
	}
public:
	typedef boost::shared_ptr<tcp_connection> pointer;

	static pointer create(boost::asio::io_context& io_context) {
		return pointer(new tcp_connection(io_context));
	}

	tcp::socket& socket() {
		return socket_;
	}

	void start() {
		message_ = make_daytime_string();

		boost::asio::async_write(socket_, boost::asio::buffer(message_), boost::bind(&tcp_connection::handle_write, shared_from_this()));
	}

private:
	tcp_connection(boost::asio::io_context& io_context) : socket_(io_context) {}

	void handle_write() {}

	tcp::socket socket_;
	std::string message_;
};

class tcp_server {
public:
	tcp_server(boost::asio::io_context& io_context) :
		io_context_(io_context),
		acceptor_(io_context, tcp::endpoint(tcp::v4(), 13)) {
		start_accept();
	}

private:
	void start_accept()	{
		tcp_connection::pointer new_connection = tcp_connection::create(io_context_);

		acceptor_.async_accept(
			new_connection->socket(), 
			boost::bind(&tcp_server::handle_accept, 
				this, 
				new_connection, 
				boost::asio::placeholders::error)
		);
	}

	void handle_accept(tcp_connection::pointer new_connection,
		const boost::system::error_code& error)	{
		if (!error) {
			new_connection->start();
		}

		start_accept();
	}

	boost::asio::io_context& io_context_;
	tcp::acceptor acceptor_;
};

class udp_server
{
	std::string make_daytime_string() {
		using namespace std; // For time_t, time and ctime;
		time_t now = time(0);
		return ctime(&now);
	}

public:
	udp_server(boost::asio::io_context& io_context) : 
		socket_(io_context, udp::endpoint(udp::v4(), 13)) {
		start_receive();
	}

private:
	void start_receive() {
		socket_.async_receive_from(
			boost::asio::buffer(recv_buffer_), 
			remote_endpoint_,
			boost::bind(&udp_server::handle_receive, this, boost::asio::placeholders::error)
		);
	}

	void handle_receive(const boost::system::error_code& error) {
		if (!error) {
			boost::shared_ptr<std::string> message(new std::string(make_daytime_string()));

			socket_.async_send_to(boost::asio::buffer(*message),
				remote_endpoint_,
				boost::bind(&udp_server::handle_send, this, message)
			);

			start_receive();
		}
	}

	void handle_send(boost::shared_ptr<std::string> /*message*/) {}

	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	boost::array<char, 1> recv_buffer_;
};
