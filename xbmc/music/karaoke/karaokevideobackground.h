#ifndef KARAOKEVIDEOFFMPEG_H
#define KARAOKEVIDEOFFMPEG_H

/*
 *      Copyright (C) 2005-2010 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "DllAvFormat.h"
#include "DllAvCodec.h"
#include "DllAvUtil.h"
#include "DllSwScale.h"

class CBaseTexture;

// C++ Interface: karaokevideoffmpeg
// Contact: oldnemesis
//
// FFMpeg-based background video decoder for Karaoke background.
// We are not using DVDPlayer for this because:
// 1. DVDPlayer was not designed to run at the same time when music is being played and other things (like lyrics) rendered. 
// While this setup works from time to time, it constantly gets broken. Some modes, like VDPAU, lead to crash right away.
//
// 2. We do not need to decode audio, hence we don't have to use extra CPU.
//
// 3. We do not really care about frame rate. Jerky video is fine for the background. Lyrics sync is much more important.
//
class KaraokeVideoFFMpeg
{
public:
  KaraokeVideoFFMpeg();
  ~KaraokeVideoFFMpeg();
  
  // Initialize the object. This function is called only once when the object is created. If it returns false, 
  // the video rendering is disabled and KaraokeVideoFFMpeg object is deleted
  bool Init();
  
  // Start playing the video. It is called each time a new song is being played. Should continue playing existing 
  // video from the position it was paused. If it returns false, the video rendering is disabled and 
  // KaraokeVideoFFMpeg object is deleted. Must write the reason for failure into the log file.
  bool Start( const CStdString& filename = "" );

  // Render the current frame into the screen. This function also must handle video loops and 
  // switching to the next video when necessary. Hence it shouldn't take too long.
  void Render();

  // Stops playing the video. It is called once the song is finished and the Render() is not going to be called anymore.
  // The object, however, is kept and should keep its state because it must continue on next Start() call.
  void Stop();

private:
  bool openVideoFile( const CStdString& filename );
  void closeVideoFile();
  bool readFrame( int frame );
  
  // FFMpeg objects
  DllAvFormat     m_dllAvFormat;
  DllAvCodec      m_dllAvCodec;
  DllAvUtil       m_dllAvUtil;
  DllSwScale      m_dllSwScale;

  CStdString      m_curVideoFile;
  unsigned int	  skipFrames;
  AVFormatContext *pFormatCtx;
  int             videoStream;
  AVCodecContext  *pCodecCtx;
  AVCodec         *pCodec;
  AVFrame         *pFrame;
  AVPicture       *pFrameRGB;  
  int              m_width;
  int              m_height;
  
  int              m_maxFrame;
  int              m_fps_den;
  int              m_fps_num;
  int              m_currentFrameNumber;
  unsigned int     m_lastTimeFrame;
  unsigned int     m_timeBase;
  
  CBaseTexture    *m_texture;
};

#endif