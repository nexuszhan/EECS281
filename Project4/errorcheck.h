// Project Identifier: 9B734EC0C043C5A836EA0EBE4BEFEA164490B2C7

#ifndef ERRORCHECK_H
#define ERRORCHECK_H

#include <iostream>

class NeedHelp
{
    public:
        void print() const
        {
            std::cout << "Try [--mode/-m] [MST/FASTTSP/OPTTSP] and give some vertices.\n";
        }
};

class ErrorCheck
{
    public:
        virtual void print() const = 0;
};

class InvalidCommand : public ErrorCheck
{
    public:
        void print() const
        {
            std::cout << "Error: Invalid command line option\n";
        }
};

class NoMode : public ErrorCheck
{
    public:
        void print() const
        {
            std::cout << "Error: No mode specified\n";
        }
};

class InvalidMode : public ErrorCheck
{
    public:
        void print() const
        {
            std::cout << "Error: Invalid mode\n";
        }
};

class NoMST
{
    public:
        void print() const 
        {
            std::cout << "Cannot construct MST\n";
        }
};

#endif