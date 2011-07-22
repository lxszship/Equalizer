
/* Copyright (c) 2006-2011, Stefan Eilemann <eile@equalizergraphics.com> 
 *                    2011, Daniel Pfeifer <daniel@pfeifer-mail.de>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 2.1 as published
 * by the Free Software Foundation.
 *  
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef EQ_WINDOWSYSTEM_H
#define EQ_WINDOWSYSTEM_H

#include <eq/client/api.h>
#include <eq/client/types.h>

namespace eq
{

/** The interface for windowing toolkits. */
class WindowSystemIF
{
protected:
    WindowSystemIF();
    virtual ~WindowSystemIF() {}

private:
    virtual std::string getName() const = 0;

    virtual SystemWindow* createWindow(Window* window) const = 0;
    virtual SystemPipe* createPipe(Pipe* pipe) const = 0;
    virtual MessagePump* createMessagePump() const = 0;
    virtual GPUInfos discoverGPUs() const = 0;

    virtual void configInit(Node* node) const {}
    virtual void configExit(Node* node) const {}

private:
    WindowSystemIF* _next;
    friend class WindowSystem;
};

#ifndef EQ_2_0_API
enum WindowSystemEnum
{                                                                            
    WINDOW_SYSTEM_NONE = 0, // must be first                                 
    WINDOW_SYSTEM_AGL,      //!< AGL/Carbon                                  
    WINDOW_SYSTEM_GLX,      //!< GLX/X11                                     
    WINDOW_SYSTEM_WGL,      //!< WGL/Win32                                   
    WINDOW_SYSTEM_ALL       // must be last                                  
};
#endif

/** The list of possible window systems. @sa Pipe::getWindowSystem() */
class WindowSystem
{
public:
    WindowSystem();
    WindowSystem( std::string const& type );
#ifndef EQ_2_0_API
    WindowSystem( const WindowSystemEnum type );

    bool operator == ( const WindowSystemEnum other ) const;
    bool operator != ( const WindowSystemEnum other ) const;
    operator WindowSystemEnum() const;
#endif

    static bool supports( std::string const& type );

    static void configInit( Node* node );
    static void configExit( Node* node );

    std::string getName() const;

    SystemWindow* createWindow( Window* window ) const;
    SystemPipe* createPipe( Pipe* pipe ) const;
    MessagePump* createMessagePump() const;
    GPUInfos discoverGPUs() const;

    bool operator == ( const WindowSystem& other ) const;
    bool operator != ( const WindowSystem& other ) const;

    friend std::ostream& operator << ( std::ostream& os, const WindowSystem& );

private:
    const WindowSystemIF* _impl;
    void _chooseImpl( const std::string& name );
};

/** Print the window system name to the given output stream. @version 1.0 */
EQ_API std::ostream& operator << ( std::ostream& os, const WindowSystem& );

} // namespace eq

#endif // EQ_WINDOWSYSTEM_H
