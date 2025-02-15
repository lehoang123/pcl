/*
 * Software License Agreement (BSD License)
 *
 *  Point Cloud Library (PCL) - www.pointclouds.org
 *  Copyright (c) 2010-2011, Willow Garage, Inc.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the copyright holder(s) nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 */

#pragma once

#include <limits>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

namespace pcl
{
  // r,g,b, i values are from 0 to 255
  // h = [0,360]
  // s, v values are from 0 to 1
  // if s = 0 => h = 0

  /** \brief Convert a XYZRGB point type to a XYZI
    * \param[in] in the input XYZRGB point 
    * \param[out] out the output XYZI point
    */
  inline void 
  PointXYZRGBtoXYZI (const PointXYZRGB& in,
                     PointXYZI&         out)
  {
    out.x = in.x; out.y = in.y; out.z = in.z;
    out.intensity = 0.299f * static_cast <float> (in.r) + 0.587f * static_cast <float> (in.g) + 0.114f * static_cast <float> (in.b);
  }

  /** \brief Convert a RGB point type to a I
    * \param[in] in the input RGB point
    * \param[out] out the output Intensity point
    */
  inline void
  PointRGBtoI (const RGB&    in,
               Intensity&    out)
  {
    out.intensity = 0.299f * static_cast <float> (in.r) + 0.587f * static_cast <float> (in.g) + 0.114f * static_cast <float> (in.b);
  }

  /** \brief Convert a RGB point type to a I
    * \param[in] in the input RGB point
    * \param[out] out the output Intensity point
    */
  inline void
  PointRGBtoI (const RGB&    in,
               Intensity8u&  out)
  {
    out.intensity = static_cast<uint8_t>(0.299f * static_cast <float> (in.r)
                      + 0.587f * static_cast <float> (in.g) + 0.114f * static_cast <float> (in.b));
  }

  /** \brief Convert a RGB point type to a I
    * \param[in] in the input RGB point
    * \param[out] out the output Intensity point
    */
  inline void
  PointRGBtoI (const RGB&    in,
               Intensity32u& out)
  {
    out.intensity = static_cast<uint32_t>(0.299f * static_cast <float> (in.r)
                      + 0.587f * static_cast <float> (in.g) + 0.114f * static_cast <float> (in.b));
  }

  /** \brief Convert a XYZRGB point type to a XYZHSV
    * \param[in] in the input XYZRGB point 
    * \param[out] out the output XYZHSV point
    */
  inline void 
  PointXYZRGBtoXYZHSV (const PointXYZRGB& in,
                       PointXYZHSV&       out)
  {
    const unsigned char max = std::max (in.r, std::max (in.g, in.b));
    const unsigned char min = std::min (in.r, std::min (in.g, in.b));

    out.x = in.x; out.y = in.y; out.z = in.z;
    out.v = static_cast <float> (max) / 255.f;

    if (max == 0) // division by zero
    {
      out.s = 0.f;
      out.h = 0.f; // h = -1.f;
      return;
    }

    const float diff = static_cast <float> (max - min);
    out.s = diff / static_cast <float> (max);

    if (min == max) // diff == 0 -> division by zero
    {
      out.h = 0;
      return;
    }

    if      (max == in.r) out.h = 60.f * (      static_cast <float> (in.g - in.b) / diff);
    else if (max == in.g) out.h = 60.f * (2.f + static_cast <float> (in.b - in.r) / diff);
    else                  out.h = 60.f * (4.f + static_cast <float> (in.r - in.g) / diff); // max == b

    if (out.h < 0.f) out.h += 360.f;
  }

  /** \brief Convert a XYZRGBA point type to a XYZHSV
    * \param[in] in the input XYZRGBA point
    * \param[out] out the output XYZHSV point
    * \todo include the A parameter but how?
    */
  inline void
  PointXYZRGBAtoXYZHSV (const PointXYZRGBA& in,
                        PointXYZHSV&        out)
  {
    const unsigned char max = std::max (in.r, std::max (in.g, in.b));
    const unsigned char min = std::min (in.r, std::min (in.g, in.b));

    out.x = in.x; out.y = in.y; out.z = in.z;
    out.v = static_cast <float> (max) / 255.f;

    if (max == 0) // division by zero
    {
      out.s = 0.f;
      out.h = 0.f;
      return;
    }

    const float diff = static_cast <float> (max - min);
    out.s = diff / static_cast <float> (max);

    if (min == max) // diff == 0 -> division by zero
    {
      out.h = 0;
      return;
    }

    if      (max == in.r) out.h = 60.f * (      static_cast <float> (in.g - in.b) / diff);
    else if (max == in.g) out.h = 60.f * (2.f + static_cast <float> (in.b - in.r) / diff);
    else                  out.h = 60.f * (4.f + static_cast <float> (in.r - in.g) / diff); // max == b

    if (out.h < 0.f) out.h += 360.f;
  }

  /* \brief Convert a XYZHSV point type to a XYZRGB
    * \param[in] in the input XYZHSV point 
    * \param[out] out the output XYZRGB point
    */
  inline void 
  PointXYZHSVtoXYZRGB (const PointXYZHSV&  in,
                       PointXYZRGB&        out)
  {
    out.x = in.x; out.y = in.y; out.z = in.z;
    if (in.s == 0)
    {
      out.r = out.g = out.b = static_cast<uint8_t> (255 * in.v);
      return;
    } 
    float a = in.h / 60;
    int   i = static_cast<int> (std::floor (a));
    float f = a - static_cast<float> (i);
    float p = in.v * (1 - in.s);
    float q = in.v * (1 - in.s * f);
    float t = in.v * (1 - in.s * (1 - f));

    switch (i)
    {
      case 0:
      {
        out.r = static_cast<uint8_t> (255 * in.v);
        out.g = static_cast<uint8_t> (255 * t);
        out.b = static_cast<uint8_t> (255 * p);
        break;
      }
      case 1:
      {
        out.r = static_cast<uint8_t> (255 * q); 
        out.g = static_cast<uint8_t> (255 * in.v); 
        out.b = static_cast<uint8_t> (255 * p); 
        break;
      }
      case 2:
      {
        out.r = static_cast<uint8_t> (255 * p);
        out.g = static_cast<uint8_t> (255 * in.v);
        out.b = static_cast<uint8_t> (255 * t);
        break;
      }
      case 3:
      {
        out.r = static_cast<uint8_t> (255 * p);
        out.g = static_cast<uint8_t> (255 * q);
        out.b = static_cast<uint8_t> (255 * in.v);
        break;
      }
      case 4:
      {
        out.r = static_cast<uint8_t> (255 * t);
        out.g = static_cast<uint8_t> (255 * p); 
        out.b = static_cast<uint8_t> (255 * in.v); 
        break;
      }
      default:
      {
        out.r = static_cast<uint8_t> (255 * in.v); 
        out.g = static_cast<uint8_t> (255 * p); 
        out.b = static_cast<uint8_t> (255 * q);
        break;
      }      
    }
  }

  /** \brief Convert a RGB point cloud to an Intensity
    * \param[in] in the input RGB point cloud
    * \param[out] out the output Intensity point cloud
    */
  inline void
  PointCloudRGBtoI (const PointCloud<RGB>&  in,
                    PointCloud<Intensity>&  out)
  {
    out.width   = in.width;
    out.height  = in.height;
    for (const auto &point : in.points)
    {
      Intensity p;
      PointRGBtoI (point, p);
      out.points.push_back (p);
    }
  }

  /** \brief Convert a RGB point cloud to an Intensity
    * \param[in] in the input RGB point cloud
    * \param[out] out the output Intensity point cloud
    */
  inline void
  PointCloudRGBtoI (const PointCloud<RGB>&    in,
                    PointCloud<Intensity8u>&  out)
  {
    out.width   = in.width;
    out.height  = in.height;
    for (const auto &point : in.points)
    {
      Intensity8u p;
      PointRGBtoI (point, p);
      out.points.push_back (p);
    }
  }

  /** \brief Convert a RGB point cloud to an Intensity
    * \param[in] in the input RGB point cloud
    * \param[out] out the output Intensity point cloud
    */
  inline void
  PointCloudRGBtoI (const PointCloud<RGB>&     in,
                    PointCloud<Intensity32u>&  out)
  {
    out.width   = in.width;
    out.height  = in.height;
    for (const auto &point : in.points)
    {
      Intensity32u p;
      PointRGBtoI (point, p);
      out.points.push_back (p);
    }
  }

  /** \brief Convert a XYZRGB point cloud to a XYZHSV
    * \param[in] in the input XYZRGB point cloud
    * \param[out] out the output XYZHSV point cloud
    */
  inline void 
  PointCloudXYZRGBtoXYZHSV (const PointCloud<PointXYZRGB>& in,
                            PointCloud<PointXYZHSV>&       out)
  {
    out.width   = in.width;
    out.height  = in.height;
    for (const auto &point : in.points)
    {
      PointXYZHSV p;
      PointXYZRGBtoXYZHSV (point, p);
      out.points.push_back (p);
    }
  }

  /** \brief Convert a XYZRGB point cloud to a XYZHSV
    * \param[in] in the input XYZRGB point cloud
    * \param[out] out the output XYZHSV point cloud
    */
  inline void
  PointCloudXYZRGBAtoXYZHSV (const PointCloud<PointXYZRGBA>& in,
                             PointCloud<PointXYZHSV>&        out)
  {
    out.width   = in.width;
    out.height  = in.height;
    for (const auto &point : in.points)
    {
      PointXYZHSV p;
      PointXYZRGBAtoXYZHSV (point, p);
      out.points.push_back (p);
    }
  }

  /** \brief Convert a XYZRGB point cloud to a XYZI
    * \param[in] in the input XYZRGB point cloud
    * \param[out] out the output XYZI point cloud
    */
  inline void 
  PointCloudXYZRGBtoXYZI (const PointCloud<PointXYZRGB>& in,
                          PointCloud<PointXYZI>&         out)
  {
    out.width   = in.width;
    out.height  = in.height;
    for (const auto &point : in.points)
    {
      PointXYZI p;
      PointXYZRGBtoXYZI (point, p);
      out.points.push_back (p);
    }
  }

  /** \brief Convert registered Depth image and RGB image to PointCloudXYZRGBA
   *  \param[in] depth the input depth image as intensity points in float
   *  \param[in] image the input RGB image
   *  \param[in] focal the focal length
   *  \param[out] out the output pointcloud
   *  **/
  inline void
  PointCloudDepthAndRGBtoXYZRGBA (const PointCloud<Intensity>&  depth,
                                  const PointCloud<RGB>&        image,
                                  const float&                  focal,
                                  PointCloud<PointXYZRGBA>&     out)
  {
    float bad_point = std::numeric_limits<float>::quiet_NaN();
    size_t width_ = depth.width;
    size_t height_ = depth.height;
    float constant_ = 1.0f / focal;

    for (size_t v = 0; v < height_; v++)
    {
      for (size_t u = 0; u < width_; u++)
      {
        PointXYZRGBA pt;
        float depth_ = depth.at (u, v).intensity;

        if (depth_ == 0)
        {
          pt.x = pt.y = pt.z = bad_point;
        }
        else
        {
          pt.z = depth_ * 0.001f;
          pt.x = static_cast<float> (u) * pt.z * constant_;
          pt.y = static_cast<float> (v) * pt.z * constant_;
        }
        pt.r = image.at (u, v).r;
        pt.g = image.at (u, v).g;
        pt.b = image.at (u, v).b;

        out.points.push_back (pt);
      }
    }
    out.width = width_;
    out.height = height_;
  }
}
