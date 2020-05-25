//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
//using namespace std;

class tcp_client {
    std::string received;

    // tcp::resolver resolver_;
    boost::asio::io_context& io_context_;
    tcp::socket socket_;
public:
    tcp_client(boost::asio::io_context& ioc) : io_context_(ioc), socket_(nullptr) {
        //socket_= tcp::socket(&(io_context_));
        try {
            //boost::asio::io_context io_context;
            tcp::resolver resolver(io_context_);

            //resolver_ = tcp::resolver(io_context_);
            tcp::resolver::results_type endpoints = resolver.resolve("localhost", "daytime");
            //tcp::resolver::results_type endpoints = resolver.resolve("192.168.1.254", "daytime");

            auto socket = tcp::socket(io_context_);
            socket_ = std::move(socket);
            //socket_(io_context_);
            boost::asio::connect(socket_, endpoints);

            receive();

            std::cout << received << "\n";
        }
        catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void receive() {
        received.clear();

        while (true) {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket_.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof) {
                break; // Connection closed cleanly by peer.
            }
            else if (error) {
                std::cout << "here1";

                throw boost::system::system_error(error); // Some other error.

            }
            received.append(buf.data(), 0, len);
            //std::cout.write(buf.data(), len);
        }
    }
};
