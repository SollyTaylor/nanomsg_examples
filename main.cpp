/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Achille Roussel <achille.roussel@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <iostream>
#include <nnxx/message>
#include <nnxx/socket>
#include <nnxx/testing>

#include <vector>
#include <sstream>


#define REPREQ   0
#define PUBSUB   1
#define PIPELINE 2

void run_repreq(int argc, char* argv[]);
void run_pubsub(int argc, char* argv[]);
void run_pipeline(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    int working_mode = REPREQ;
//    int working_mode = PUBSUB;
//    int working_mode = PIPELINE;


    if (working_mode == REPREQ)
        run_repreq(argc, argv);
    else if(working_mode == PUBSUB)
        run_pubsub(argc, argv);
    else if(working_mode == PIPELINE)
        run_pipeline(argc, argv);
    return 0;

}


void run_repreq(int argc, char* argv[])
{
//    std::cout << argc << std::endl;
    if (argc != 2)
    {
        std::cout << "Usage:  \nfor server:  $ ./nnxx server\n";
        std::cout << "for client:  $ ./nnxx client\n";
        exit(-1);
    }
    if (std::string(argv[1]) == "server")
    {
        nnxx::socket s1 { nnxx::SP, nnxx::REP };
        std::cout << "I am server" << std::endl;
        s1.bind("tcp://*:18913");
        std::string rcvstr = s1.recv<std::string>();
        std::cout << "Server received " << rcvstr << std::endl;
        //        std::string rcvstr = "blablabla";
        s1.send(std::string{rcvstr});
    }
    else if (std::string(argv[1]) == "client")
    {
        nnxx::socket s2 { nnxx::SP, nnxx::REQ };
        std::cout << "I am client" << std::endl;
        s2.connect("tcp://127.0.0.1:18913");
        std::string input = "Hello nanomsg, test repreq";
        s2.send(input);
        std::cout << "Client req " << input << std::endl;
        std::string rcvstr = to_string(s2.recv());
        std::cout << "Client ack " << rcvstr << std::endl;
    }
}

void run_pubsub(int argc, char* argv[])
{
    if (argc != 2 && argc != 3)
    {
        std::cout << "Usage: \n "
                  << "for server:  $ ./nnxx server\n";
        std::cout << "for client:  $ ./nnxx client <any_sub_name>\n";
        exit(-1);
    }

    if (std::string(argv[1]) == "server")
    {
        if (argc != 2)
        {
            std::cout << "Usage: \n "
                      << "for server:  $ ./nnxx server\n";
            exit(-1);
        }
        nnxx::socket syncsrv { nnxx::SP, nnxx::REP };
        syncsrv.bind("tcp://*:5557");
        nnxx::socket s1 { nnxx::SP, nnxx::PUB };
        s1.bind("tcp://*:5556");
        std::cout << "I am pub server" << std::endl;

        int n_subs = 2;
        std::cout
                << "sync server on, totally "
                << n_subs
                << "subscribers demanded."
                << std::endl;

        std::vector<std::string> subs;
        while (subs.size()<n_subs)
        {
            std::string sub = syncsrv.recv<std::string>();
            if (std::find(subs.begin(), subs.end(), sub) == subs.end())
            {
                subs.push_back(sub);
                std::string rcvstr = "ack: " + sub;
                syncsrv.send(std::string{rcvstr});
                std::cout << rcvstr << std::endl;
            }
        }

        for (int i = 0; i<100; i++)
        {
            std::stringstream ss;
            ss << "HW: " << i+1;
            s1.send(ss.str());
//            std::cin.get();
        }
        s1.send("END");
    }
    else if (std::string(argv[1]) == "client")
    {
        if (argc != 3)
        {
            std::cout << "Usage: \n "
                      << "for client:  $ ./nnxx client <any_sub_name>\n";
            exit(-1);
        }
        nnxx::socket syncsrv { nnxx::SP, nnxx::REQ };
        syncsrv.connect("tcp://127.0.0.1:5557");

        nnxx::socket s2 { nnxx::SP, nnxx::SUB };
        s2.connect("tcp://127.0.0.1:5556");
        nnxx::subscribe(s2);

        std::cout << "sync client on" << std::endl;
        std::string subname(argv[2]);
        syncsrv.send(subname);
        std::string ackstr = syncsrv.recv<std::string>();
        std::cout << ackstr << std::endl;

        std::cout << "I am sub client" << std::endl;
        std::cout << "Receiving..." << std::endl;
        while(true)
        {
            std::string rcvstr = to_string(s2.recv());
            std::cout << rcvstr << std::endl;
            if (rcvstr == "END") break;
        }
    }
}


void run_pipeline(int argc, char* argv[])
{
    if (argc != 2 && argc != 3)
    {
        std::cout << "Usage: \n "
                  << "for server:  $ ./nnxx server\n";
        std::cout << "for client:  $ ./nnxx client <any_sub_name>\n";
        exit(-1);
    }

    if (std::string(argv[1]) == "server")
    {
        if (argc != 2)
        {
            std::cout << "Usage: \n "
                      << "for server:  $ ./nnxx server\n";
            exit(-1);
        }
        nnxx::socket syncsrv { nnxx::SP, nnxx::REP };
        syncsrv.bind("tcp://*:5557");
        nnxx::socket s1 { nnxx::SP, nnxx::PUSH };
        s1.bind("tcp://*:5556");
        std::cout << "I am a push server" << std::endl;

        std::cout << "sync server on" << std::endl;
        int n_subs = 2;
        std::vector<std::string> subs;
        while (subs.size()<n_subs)
        {
            std::string sub = syncsrv.recv<std::string>();
            if (std::find(subs.begin(), subs.end(), sub) == subs.end())
            {
                subs.push_back(sub);
                std::string rcvstr = "ack: " + sub;
                syncsrv.send(std::string{rcvstr});
                std::cout << rcvstr << std::endl;
            }
        }

        for (int i = 0; i<100; i++)
        {
            std::stringstream ss;
            ss << "HW: " << i+1;
            s1.send(ss.str());
//            std::cin.get();
        }
        for (int j = 0; j<n_subs; j++)
        {
            s1.send("END");
        }
    }
    else if (std::string(argv[1]) == "client")
    {
        if (argc != 3)
        {
            std::cout << "Usage: \n "
                      << "for client:  $ ./nnxx client <any_worker_name>\n";
            exit(-1);
        }
        nnxx::socket syncsrv { nnxx::SP, nnxx::REQ };
        syncsrv.connect("tcp://127.0.0.1:5557");

        nnxx::socket s2 { nnxx::SP, nnxx::PULL };
        s2.connect("tcp://127.0.0.1:5556");
//        nnxx::subscribe(s2);

        std::cout << "sync client on" << std::endl;
        std::string subname(argv[2]);
        syncsrv.send(subname);
        std::string ackstr = syncsrv.recv<std::string>();
        std::cout << ackstr << std::endl;

        std::cout << "I am pull client" << std::endl;
        std::cout << "Receiving..." << std::endl;
        while(true)
        {
            std::string rcvstr = to_string(s2.recv());
            std::cout << rcvstr << std::endl;
            if (rcvstr == "END") break;
        }
    }
}
