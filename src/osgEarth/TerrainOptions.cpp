/* -*-c++-*- */
/* osgEarth - Dynamic map generation toolkit for OpenSceneGraph
 * Copyright 2008-2010 Pelican Mapping
 * http://osgearth.org
 *
 * osgEarth is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include <osgEarth/TerrainOptions>
#include <osg/Notify>
#include <OpenThreads/Thread>

using namespace osgEarth;

std::string TerrainOptions::OPTIONS_TAG = "__osgEarth::TerrainOptions";

//------------------------------------------------------------------------

LoadingPolicy::LoadingPolicy( const Config& conf ) :
_mode( MODE_STANDARD ),
_numThreads( 2 ),
_numThreadsPerCore( 4 ),
_numTileGenThreads( OpenThreads::GetNumberOfProcessors() )
{
    mergeConfig( conf );
}

void
LoadingPolicy::mergeConfig( const Config& conf )
{
    conf.getIfSet( "mode", "standard", _mode, MODE_STANDARD );
    conf.getIfSet( "mode", "sequential", _mode, MODE_SEQUENTIAL );
    conf.getIfSet( "mode", "preemptive", _mode, MODE_PREEMPTIVE );
    conf.getIfSet( "loading_threads", _numThreads );
    conf.getIfSet( "loading_threads_per_logical_processor", _numThreadsPerCore );
    conf.getIfSet( "loading_threads_per_core", _numThreadsPerCore );
    conf.getIfSet( "tile_generation_threads", _numTileGenThreads );
}

Config
LoadingPolicy::getConfig() const
{
    Config conf( "loading_policy" );
    conf.addIfSet( "mode", "standard", _mode, MODE_STANDARD );
    conf.addIfSet( "mode", "sequential", _mode, MODE_SEQUENTIAL );
    conf.addIfSet( "mode", "preemptive", _mode, MODE_PREEMPTIVE );
    conf.addIfSet( "loading_threads", _numThreads );
    //conf.addIfSet( "loading_threads_per_logical_processor", _numThreadsPerCore );
    conf.addIfSet( "loading_threads_per_core", _numThreadsPerCore );
    conf.addIfSet( "tile_generation_threads", _numTileGenThreads );
    return conf;
}

//----------------------------------------------------------------------------

TerrainOptions::TerrainOptions( const Config& conf ) :
DriverConfigOptions( conf ),
_loadingPolicy( LoadingPolicy() ),
_verticalScale( 1.0f ),
//_heightFieldSkirtRatio( 0.05f ),
_heightFieldSampleRatio( 1.0f ),
_minTileRangeFactor( 6.0 ),
_normalizeEdges( false ),
_combineLayers( true ),
_maxLOD( 23 ),
_layeringTechnique( LAYERING_MULTITEXTURE ),
_enableLighting( true ),
_elevationInterpolation( INTERP_BILINEAR )
{
    mergeConfig( conf );
}

Config
TerrainOptions::getConfig() const
{
    Config conf = DriverConfigOptions::getConfig();
    conf.key() = "terrain_options";

    conf.addObjIfSet( "loading_policy", _loadingPolicy );
    conf.addIfSet( "vertical_scale", _verticalScale );
    //conf.addIfSet( "skirt_ratio", _heightFieldSkirtRatio );
    conf.addIfSet( "sample_ratio", _heightFieldSampleRatio );
    conf.addIfSet( "min_tile_range_factor", _minTileRangeFactor );
    conf.addIfSet( "normalize_edges", _normalizeEdges );
    conf.addIfSet( "combine_layers", _combineLayers );
    conf.addIfSet( "max_lod", _maxLOD );
    conf.addIfSet( "lighting", _enableLighting );

    conf.addIfSet( "layering_technique", "multipass", _layeringTechnique, LAYERING_MULTIPASS );
    conf.addIfSet( "layering_technique", "multitexture", _layeringTechnique, LAYERING_MULTITEXTURE );
    conf.addIfSet( "layering_technique", "composite", _layeringTechnique, LAYERING_COMPOSITE );

    conf.addIfSet( "elevation_interpolation", "nearest",     _elevationInterpolation, INTERP_NEAREST);
    conf.addIfSet( "elevation_interpolation", "average",     _elevationInterpolation, INTERP_AVERAGE);
    conf.addIfSet( "elevation_interpolation", "bilinear",    _elevationInterpolation, INTERP_BILINEAR);
    conf.addIfSet( "elevation_interpolation", "triangulate", _elevationInterpolation, INTERP_TRIANGULATE);

    return conf;
}

void
TerrainOptions::mergeConfig( const Config& conf )
{
    DriverConfigOptions::mergeConfig( conf );

    conf.getObjIfSet( "loading_policy", _loadingPolicy );
    conf.getIfSet( "vertical_scale", _verticalScale );
    //conf.getIfSet( "skirt_ratio", _heightFieldSkirtRatio );
    conf.getIfSet( "sample_ratio", _heightFieldSampleRatio );
    conf.getIfSet( "min_tile_range_factor", _minTileRangeFactor );
    conf.getIfSet( "normalize_edges", _normalizeEdges );
    conf.getIfSet( "combine_layers", _combineLayers );
    conf.getIfSet( "max_lod", _maxLOD );
    conf.getIfSet( "lighting", _enableLighting );

    conf.getIfSet( "layering_technique", "multipass", _layeringTechnique, LAYERING_MULTIPASS );
    conf.getIfSet( "layering_technique", "multitexture", _layeringTechnique, LAYERING_MULTITEXTURE );
    conf.getIfSet( "layering_technique", "composite", _layeringTechnique, LAYERING_COMPOSITE );

    conf.getIfSet( "elevation_interpolation", "nearest",     _elevationInterpolation, INTERP_NEAREST);
    conf.getIfSet( "elevation_interpolation", "average",     _elevationInterpolation, INTERP_AVERAGE);
    conf.getIfSet( "elevation_interpolation", "bilinear",    _elevationInterpolation, INTERP_BILINEAR);
    conf.getIfSet( "elevation_interpolation", "triangulate", _elevationInterpolation, INTERP_TRIANGULATE);
}
