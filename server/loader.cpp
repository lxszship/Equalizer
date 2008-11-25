
/* Copyright (c) 2006-2008, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#include "loader.h" 

#include "channel.h" 
#include "compound.h" 
#include "node.h"    
#include "pipe.h"    
#include "server.h"
#include "window.h"  

using namespace std;

namespace eq
{
namespace server
{

//---------------------------------------------------------------------------
// factory methods
//---------------------------------------------------------------------------
ServerPtr Loader::createServer()
{
    return new Server;
}

Config*   Loader::createConfig()
{
    return new Config;
}

Node*     Loader::createNode()
{
    return new Node;
}

Pipe*     Loader::createPipe()
{
    return new Pipe;
}

Window*   Loader::createWindow()
{
    return new Window;
}

Channel*  Loader::createChannel()
{
    return new Channel;
}

Compound* Loader::createCompound()
{
    return new Compound;
}

}
}
