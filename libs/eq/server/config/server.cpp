
/* Copyright (c) 2011-2012, Stefan Eilemann <eile@eyescale.h>
 *                    2012, Daniel Nachbaur <danielnachbaur@gmail.com>
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

#include "server.h"

#include "display.h"
#include "resources.h"

#include "../config.h"
#include "../global.h"
#include "../loader.h"
#include "../server.h"

#include <fstream>

namespace eq
{
namespace server
{
namespace config
{

Config* Server::configure( ServerPtr server, const std::string& session,
                           const uint32_t flags )
{
    if( !server->getConfigs().empty( )) // don't do more than one auto config
        return 0;

    Global::instance()->setConfigFAttribute( Config::FATTR_VERSION, 1.2f );

    Config* config = new Config( server );
    config->setName( session + " autoconfig" );

    if( !Resources::discover( config, session, flags ))
    {
        delete config;
        return 0;
    }

    if( config->getNodes().size() > 1 ) // add server connection for clusters
    {
        co::ConnectionDescriptionPtr desc = new co::ConnectionDescription;
        desc->port = EQ_DEFAULT_PORT;
        server->addListener( desc );
    }

    Display::discoverLocal( config, flags );
    const Compounds compounds = Loader::addOutputCompounds( server );
    if( compounds.empty( ))
    {
        delete config;
        return 0;
    }

    const Channels channels = Resources::configureSourceChannels( config );
    Resources::configure( compounds, channels, flags );

    std::ofstream configFile;
    const std::string filename = session + ".auto.eqc";
    configFile.open( filename.c_str( ));
    if( configFile.is_open( ))
    {
        std::ostream& previous = lunchbox::Log::getOutput();

        lunchbox::Log::setOutput( configFile );
        lunchbox::Log::instance( __FILE__, __LINE__ )
            << lunchbox::disableHeader << Global::instance() << *server
            << std::endl << lunchbox::enableHeader;
        lunchbox::Log::setOutput( previous );

        configFile.close();
    }
    return config;
}

}
}
}
