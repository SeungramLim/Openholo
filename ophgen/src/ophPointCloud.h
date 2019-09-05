/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install, copy or use the software.
//
//
//                           License Agreement
//                For Open Source Digital Holographic Library
//
// Openholo library is free software;
// you can redistribute it and/or modify it under the terms of the BSD 2-Clause license.
//
// Copyright (C) 2017-2024, Korea Electronics Technology Institute. All rights reserved.
// E-mail : contact.openholo@gmail.com
// Web : http://www.openholo.org
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//  1. Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//  2. Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the copyright holder or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
// This software contains opensource software released under GNU Generic Public License,
// NVDIA Software License Agreement, or CUDA supplement to Software License Agreement.
// Check whether software you use contains licensed software.
//
//M*/

#ifndef __ophPointCloud_h
#define __ophPointCloud_h

#define _USE_MATH_DEFINES

#include "ophGen.h"

//Build Option : Multi Core Processing (OpenMP)
#ifdef _OPENMP
#include <omp.h>
#endif

/* Bitmap File Definition*/
#define OPH_Bitsperpixel 8 //24 // 3byte=24 
#define OPH_Planes 1
#define OPH_Compression 0
#define OPH_Xpixelpermeter 0x130B //2835 , 72 DPI
#define OPH_Ypixelpermeter 0x130B //2835 , 72 DPI
#define OPH_Pixel 0xFF

using namespace oph;

/**
* @addtogroup pointcloud
//@{
* @detail

* @section Introduction

This module is related methods which generates CGH based on 3D point cloud. It is supported single core
processing, multi-core processing(with OpenMP) and GPGPU parallel processing(with CUDA).

![Fig1.Hologram generation by object beam and reference beam](pics/ophgen/pointcloud/pointcloud_RS_1.png)
	-Fig1.Hologram generation by object beam and reference beam

I. Point Cloud Hologram Generation with Rayleigh-Sommerfeld(RS) Integral

Reference wave :
\f[
	R
	\left(
		\xi,\eta
	\right)
	=
	a_{R} \exp
	\left\{
		-jk
		\left(
			\xi \sin \theta_{\xi}
			+
			\eta \sin \theta_{\eta}
		\right)
	\right\}
\f]


Object wave :

\f[
	O
	\left(
		\xi,\eta
	\right)
	=
	\sum_{p=1}^{N}
	\frac{a_{p}}{r_{p}}
	\exp
	\left\{
		j
		\left(
			kr_{p} + \phi_{p}
		\right)
	\right\}
\f]


Distance :

\f[
	r_{p}
	=
	\sqrt{
		\left(
			\xi - x_{p}
		\right)^{2}
		+
		\left(
			\eta - y_{p}
		\right)^{2}
		+
		z_{p}^{2}
	}
\f]


Intensity :

As shown in Fig. 1, when the object wave and the reference wave are overlapped with each other, an interference pattern is generated by each other. \n
Therefore, the interference pattern of the hologram plate
The intensity, I, is:

\f[
	I
	\left(
		\xi,\eta
	\right)
	=
	\left|
		O
		\left(
			\xi,\eta
		\right)
		+
		R
		\left(
			\xi,\eta
		\right)
	\right|^{2}
	=
	\left|
		O
	\right|^{2}
	+
	\left|
		R
	\right|^{2}
	+
	R^{*} O + R O^{*}
\f]

Since the \f$\left|O\right|^{2} + \left|R\right|^{2}\f$ in this case does not have three-dimensional information as a DC term, it is not used, and \f$R^{*}O + RO^{*}\f$, which is the following two terms, has hologram information.



\n
The interference pattern generated on the hologram is calculated by the interference between the object wave and the reference wave as in the above equation.\n
Here, the \f$\left|O\right|^{2} + \left|R\right|^{2}\f$ formula is the intensity distribution of the object wave and the reference wave, which is irrelevant to the image of the object, but the \f$R^{*}O + RO^{*}\f$ formula has the hologram information of the object.\n
When calculating the interference pattern, Only a simplified calculation can be performed. If you select this port and calculate it briefly, you will get the following formula.

\f[
	I
	\left(
		\xi,\eta
	\right)
	=
	\sum_{i=1}^{N} \frac{a_{i}}{r_{i}}
	\exp
	\left(
		kr_{i}
		+
		k \xi \sin \theta_{\xi}
		+
		k \eta \sin \theta_{\eta}
	\right)
\f]

\n
II. Point Cloud Hologram Generation with Rayleigh-Sommerfeld(RS) Integral 2

Rayleigh-Sommerfeld solution I :

\f[
	u
	\left(
		x,y
	\right)
	=
	\frac{z}{j \lambda}
	\int_{}^{} {
		\int_{\Sigma}^{} {u
			\left(
				\xi,\eta
			\right)
			\frac{ \exp
					\left(
						jkr_{01}
					\right)
				}
			{r_{01}^{2}}
		} d \xi
	} d \eta
\f]

Impulse response

\f[
	h
	\left(
		x,y
	\right)
	=
	\frac{z}{j \lambda}
	\frac{ \exp
			\left(
				jkr
			\right)
	}{r^{2}}
\f]


R-S diffraction Anti-alliasing

\f[
		\frac{1}{2\pi}
		\left|
			\frac{\partial \phi}{\partial x}
		\right|
		<
		\frac{1}{2 p_{x}}
	,\quad
		\frac{1}{2\pi}
		\left|
			\frac{\partial \phi}{\partial y}
		\right|
		<
		\frac{1}{2 p_{y}}

	\qquad \to \qquad

		\frac{1}{2\pi}
		\left|
			\frac{x - x_{o}}{r}
		\right|
		<
		\frac{1}{2 p_{x}}
	,\quad
		\frac{1}{2\pi}
		\left|
			\frac{y - y_{o}}{r}
		\right|
		<
		\frac{1}{2 p_{y}}
\f]

\f[
	\to	\qquad

	\begin{matrix}
		x_{o}
		-
		\left|
			\frac{t_{x}}{\sqrt{1 - t_{x}^{2}}}
			\sqrt{
				\left(
					y - y_{o}^{2}
				\right)
				+
				z_{o}^{2}
			}
		\right|

		\quad < \quad
		x
		\quad < \quad

		x_{o}
		+
		\left|
			\frac{t_{x}}{\sqrt{1 - t_{x}^{2}}}
			\sqrt{
				\left(
					y - y_{o}^{2}
				\right)
				+
				z_{o}^{2}
			}
		\right|
	,\\
		y_{o}
		-
		\left|
			\frac{t_{y}}{\sqrt{1 - t_{y}^{2}}}
			\sqrt{
				\left(
					x - x_{o}^{2}
				\right)
				+
				z_{o}^{2}
			}
		\right|

		\quad < \quad
		y
		\quad < \quad

		y_{o}
		+
		\left|
			\frac{t_{y}}{\sqrt{1 - t_{y}^{2}}}
			\sqrt{
				\left(
					x - x_{o}^{2}
				\right)
				+
				z_{o}^{2}
			}
		\right|
	\end{matrix}
\f]

\f[
	\texttt{where} \quad
		t_{x}
		=
		\frac{\lambda}{2p_{x}}
	,\quad
		t_{y}
		=
		\frac{\lambda}{2p_{y}}
\f]


Search Range : Rectangle

\f[
		\texttt{x direction}
	\quad \to \quad
		y
		=
		y_{o}
	\quad \to \quad
		x_{o}
		-
		\left|
			\frac{t_{x}}{\sqrt{1 - t_{x}^{2}}} z_{o}
		\right|\

		<\
		x\
		<\

		x_{o}
		+
		\left|
			\frac{t_{x}}{\sqrt{1 - t_{x}^{2}}} z_{o}
		\right|
\f]

\f[
		\texttt{y direction}
	\quad \to \quad
		x
		=
		x_{o}
	\quad \to \quad
		y_{o}
		-
		\left|
			\frac{t_{y}}{\sqrt{1 - t_{y}^{2}}} z_{o}
		\right|\

		<\
		y\
		<\

		y_{o}
		+
		\left|
			\frac{t_{y}}{\sqrt{1 - t_{y}^{2}}} z_{o}
		\right|
\f]

\n
III. Point Cloud Hologram Generation with Fresnel Diffraction Integral

Fresnel Diffraction Integral :

\f[
	u
	\left(
		x,y
	\right)

	=
	\frac{z}{j \lambda}
	\int_{}^{} {
		\int_{}^{} {
			u
			\left(
				\xi,\eta
			\right)
			\frac{ \exp
					\left(
						jkr_{01}
					\right)
				}
			{r_{01}^{2}}
		} d \xi
	} d \eta

	\cong
	\frac{e^{jkz}}{j \lambda z}
	\int_{}^{} {
		\int_{}^{} {
			u
			\left(
				\xi,\eta
			\right)
			\exp
			\left\{
				j
				\frac{k}{2z}
				\left[
					\left(
						x - \xi
					\right)^{2}
					+
					\left(
						y - \eta
					\right)^{2}
				\right]
			\right\}
		} d \xi
	} d \eta
\f]

Impulse response

\f[
	h
	\left(
		x,y
	\right)
	\cong
	\frac{e^{jkz}}{j \lambda z}
	\exp
	\left\{
		j
		\frac{k}{2z}
		\left(
			x^{2}
			+
			y^{2}
		\right)
	\right\}
\f]


Fresnel diffraction Anti-alliasing

\f[
	\begin{matrix}
		\frac{1}{2\pi}
		\left|
			\frac{\partial \phi}{\partial x}
		\right|
		<
		\frac{1}{2 p_{x}}
	,\\
		\frac{1}{2\pi}
		\left|
			\frac{\partial \phi}{\partial y}
		\right|
		<
		\frac{1}{2 p_{y}}
	\end{matrix}

	\qquad \to \qquad

	\begin{matrix}
		x_{o}
		-
		\left|
			\frac{\lambda z_{o}}{2 p_{x}}
		\right|\

		<\
		x\
		<\

		x_{o}
		+
		\left|
			\frac{\lambda z_{o}}{2 p_{x}}
		\right|
	,\\
		y_{o}
		-
		\left|
			\frac{\lambda z_{o}}{2 p_{y}}
		\right|\

		<\
		y\
		<\

		y_{o}
		+
		\left|
			\frac{\lambda z_{o}}{2 p_{y}}
		\right|
	\end{matrix}
\f]

*/
//! @} pointcloud

/**
* @ingroup pointcloud
* @brief Openholo Point Cloud based Compter-generated holography.
* @author
*/
class GEN_DLL ophPointCloud : public ophGen
{
public:
	enum PC_DIFF_FLAG {
		PC_DIFF_RS,
		PC_DIFF_FRESNEL,
	};
	/**
	* @brief Constructor
	* @details Initialize variables.
	*/
	explicit ophPointCloud(void);
	/**
	* @overload
	*/
	explicit ophPointCloud(const char*, const char* cfg_file);
protected:
	/**
	* @brief Destructor
	*/
	virtual ~ophPointCloud(void);

public:
	inline void setScale(Real sx, Real sy, Real sz) { pc_config_.scale.v[0] = sx; pc_config_.scale.v[1] = sy; pc_config_.scale.v[2] = sz; }
	inline void setOffsetDepth(Real offset_depth) { pc_config_.offset_depth = offset_depth; }
	inline void setFilterShapeFlag(int8_t* fsf) { pc_config_.filter_shape_flag = fsf; }
	inline void setFilterWidth(Real wx, Real wy) { pc_config_.filter_width.v[0] = wx; pc_config_.filter_width.v[1] = wy; }
	inline void setFocalLength(Real lens_in, Real lens_out, Real lens_eye_piece) { pc_config_.focal_length_lens_in = lens_in; pc_config_.focal_length_lens_out = lens_out; pc_config_.focal_length_lens_eye_piece = lens_eye_piece; }
	inline void setTiltAngle(Real ax, Real ay) { pc_config_.tilt_angle.v[0] = ax; pc_config_.tilt_angle.v[1] = ay; }

	inline void getScale(vec3& scale) { scale = pc_config_.scale; }
	inline Real getOffsetDepth(void) { return pc_config_.offset_depth; }
	inline int8_t* getFilterShapeFlag(void) { return pc_config_.filter_shape_flag; }
	inline void getFilterWidth(vec2& filterwidth) { filterwidth = pc_config_.filter_width; }
	inline void getFocalLength(Real* lens_in, Real* lens_out, Real* lens_eye_piece) {
		if (lens_in != nullptr) *lens_in = pc_config_.focal_length_lens_in;
		if (lens_out != nullptr) *lens_out = pc_config_.focal_length_lens_out;
		if (lens_eye_piece != nullptr) *lens_eye_piece = pc_config_.focal_length_lens_eye_piece;
	}
	inline void getTiltAngle(vec2& tiltangle) { tiltangle = pc_config_.tilt_angle; }
	inline Real** getVertex(void) { return &pc_data_.vertex; }
	inline Real** getColorPC(void) { return &pc_data_.color; }
	inline Real** getPhasePC(void) { return &pc_data_.phase; }
	inline void setPointCloudModel(Real* vertex, Real* color, Real *phase) {
		pc_data_.vertex = vertex;
		pc_data_.color = color;
		pc_data_.phase = phase;
	}
	inline void getPointCloudModel(Real *vertex, Real *color, Real *phase) {
		getModelLocation(vertex);
		getModelColor(color);
		getModelPhase(phase);
	}

	/**
	* @brief Directly Set Basic Data
	*/
	/**
	* @param Location 3D Point Cloud Geometry Data
	* @param Color 3D Point Cloud Color Data
	* @param Amplitude 3D Point Cloud Model Amplitude Data of Point-Based Light Wave
	* @param Phase 3D Point Cloud Model Phase Data of Point-Based Light Wave
	*/
	inline const Real* getModelLocation(Real *vertex) { vertex != NULL ? vertex = pc_data_.vertex : vertex; return pc_data_.vertex; }
	inline const Real* getModelColor(Real *color) { color != NULL ? color = pc_data_.color : color; return pc_data_.color; }
	inline const Real* getModelPhase(Real *phase) { phase != NULL ? phase = pc_data_.phase : phase; return pc_data_.phase; }
	inline int getNumberOfPoints() { return n_points; }
	inline Real getFieldLens(void) { return pc_config_.field_lens; }

public:
	/**
	* @brief Set the value of a variable is_CPU(true or false)
	* @details <pre>
	if is_CPU == true
	CPU implementation
	else
	GPU implementation </pre>
	* @param is_CPU : the value for specifying whether the hologram generation method is implemented on the CPU or GPU
	*/
	void setMode(bool is_CPU);

	/**
	* @brief override
	* @{
	* @brief Import Point Cloud Data Base File : *.dat file.
	* This Function is included memory location of Input Point Clouds.
	*/
	/**
	* @brief override
	* @param InputModelFile PointCloud(*.dat) input file path
	* @return number of Pointcloud (if it failed loading, it returned -1)
	*/
	int loadPointCloud(const char* pc_file);

	/**
	* @brief
	* @{
	* @brief Import Specification Config File(*.config) file
	*/
	/**
	* @param InputConfigFile Specification Config(*.config) file path
	*/
	bool readConfig(const char* cfg_file);

	/**
	* @brief Generate a hologram, main funtion.
	* @param Select diffraction flag\n
	*		PC_DIFF_RS: Diffraction using R-S integral\n
	*		PC_DIFF_FRESNEL: Diffraction using Fresnel integral
	* @return implement time (sec)
	*/
	Real generateHologram(uint diff_flag = PC_DIFF_RS);
	void encodeHologram(vec2 band_limit = vec2(0.8, 0.5), vec2 spectrum_shift = vec2(0.0, 0.5));

	virtual void encoding(unsigned int ENCODE_FLAG);
	virtual void encoding(unsigned int ENCODE_FLAG, unsigned int SSB_PASSBAND);

	/**
	* @brief Set the value of a variable is_ViewingWindow(true or false)
	* @details <pre>
	if is_ViewingWindow == true
	Transform viewing window
	else
	GPU implementation </pre>
	* @param is_TransVW : the value for specifying whether the hologram generation method is implemented on the viewing window
	*/
	void setViewingWindow(bool is_ViewingWindow);
private:
	/**
	* @brief Calculate Integral Fringe Pattern of 3D Point Cloud based Computer Generated Holography
	* @param VertexArray Input 3D PointCloud Model Coordinate Array Data
	* @param AmplitudeArray Input 3D PointCloud Model Amplitude Array Data
	* @param dst Output Fringe Pattern
	* @return implement time (sec)
	*/
	void genCghPointCloudCPU(uint diff_flag);
	void diffractEncodedRS(ivec2 pn, vec2 pp, vec2 ss, vec3 pc, Real k, Real amplitude, vec2 theta);
	void diffractNotEncodedRS(ivec2 pn, vec2 pp, vec2 ss, vec3 pc, Real k, Real amplitude, Real lambda, vec2 theta);
	void diffractEncodedFrsn(void);
	void diffractNotEncodedFrsn(ivec2 pn, vec2 pp, vec3 pc, Real amplitude, Real lambda, vec2 theta);
	Real transformViewingWindow(Real pt);
	void transformViewingWindow(int nSize, Real *dst, Real *src);
	/**
	* @overload
	* @param Model Input 3D PointCloud Model Data
	* @param dst Output Fringe Pattern
	* @return implement time (sec)
	*/
	//double genCghPointCloud(const std::vector<PointCloud> &Model, float *dst);
	/** @}	*/

	/**
	* @brief GPGPU Accelation of genCghPointCloud() using nVidia CUDA
	* @param VertexArray Input 3D PointCloud Model Coordinate Array Data
	* @param AmplitudeArray Input 3D PointCloud Model Amplitude Array Data
	* @param dst Output Fringe Pattern
	* @return implement time (sec)
	*/
	void genCghPointCloudGPU(uint diff_flag);

	/** @}	*/

	/**
	* @brief normalize calculated fringe pattern to 8bit grayscale value.
	* @param src: Input float type pointer
	* @param dst: Output char tpye pointer
	* @param nx: The number of pixels in X
	* @param ny: The number of pixels in Y
	*/
	virtual void ophFree(void);

	bool is_CPU;
	bool is_ViewingWindow;
	int n_points;

	OphPointCloudConfig pc_config_;
	OphPointCloudData	pc_data_;
};

#endif // !__ophPointCloud_h