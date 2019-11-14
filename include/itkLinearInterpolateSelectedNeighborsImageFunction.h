/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkLinearInterpolateSelectedNeighborsImageFunction_h
#define itkLinearInterpolateSelectedNeighborsImageFunction_h

#include "itkInterpolateImageFunction.h"
#include "itkVariableLengthVector.h"

namespace itk
{
/** \class LinearInterpolateSelectedNeighborsImageFunction
 * \brief Linearly interpolate an image at specified positions.
 *
 * LinearInterpolateSelectedNeighborsImageFunction linearly interpolates image intensity at
 * a non-integer pixel position. This class is templated
 * over the input image type and the coordinate representation type
 * (e.g. float or double).
 *
 * This function works for N-dimensional images.
 *
 * This function uses a functor to verify whether neighbours are valid.
 * The intention is to enable reliable estimations of pixel values in
 * the presence of indicator pixels, as generated by fast marching methods.
 *
 * It is not optimised for specific dimensions.
 *
 * \ingroup ImageFunctions ImageInterpolators
 * \ingroup ITKImageFunction
 *
 */

  namespace Functor
{

template< typename TInput >
class AllNeighbors
{
public:
  AllNeighbors() = default;
  ~AllNeighbors() = default;
  bool operator!=(const AllNeighbors &) const
  {
    return false;
  }

  bool operator==(const AllNeighbors & other) const
  {
    return !( *this != other );
  }

  inline bool operator()(const TInput & A) const
  { return true; }
};
}


  template< typename TInputImage, typename TCoordRep = double, typename TNeighborCheckFunction = Functor::AllNeighbors<typename TInputImage::PixelType> >
class ITK_TEMPLATE_EXPORT LinearInterpolateSelectedNeighborsImageFunction:
  public InterpolateImageFunction< TInputImage, TCoordRep >
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(LinearInterpolateSelectedNeighborsImageFunction);

  /** Standard class type aliases. */
  using Self = LinearInterpolateSelectedNeighborsImageFunction;
  using Superclass = InterpolateImageFunction< TInputImage, TCoordRep >;
  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;

  /** Run-time type information (and related methods). */
  itkTypeMacro(LinearInterpolateSelectedNeighborsImageFunction, InterpolateImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** OutputType type alias support */
  using OutputType = typename Superclass::OutputType;

  /** InputImageType type alias support */
  using InputImageType = typename Superclass::InputImageType;

  /** InputPixelType type alias support */
  using InputPixelType = typename Superclass::InputPixelType;

  /** RealType type alias support */
  using RealType = typename Superclass::RealType;

  /** Size type alias support */
  using SizeType = typename Superclass::SizeType;

  /** Dimension underlying input image. */
  static constexpr unsigned int ImageDimension = Superclass::ImageDimension;

  /** Index type alias support */
  using IndexType = typename Superclass::IndexType;

  /** ContinuousIndex type alias support */
  using ContinuousIndexType = typename Superclass::ContinuousIndexType;
  using InternalComputationType = typename ContinuousIndexType::ValueType;

  using NeighborCheckFunctionType = TNeighborCheckFunction;

  /** Evaluate the function at a ContinuousIndex position
   *
   * Returns the linearly interpolated image intensity at a
   * specified point position. No bounds checking is done.
   * The point is assume to lie within the image buffer.
   *
   * ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  OutputType EvaluateAtContinuousIndex(const
                                                ContinuousIndexType &
                                                index) const override
  {
    return this->EvaluateUnoptimized(index);
  }

  SizeType
  GetRadius() const override
  {
    return SizeType::Filled(1);
  }

protected:
  LinearInterpolateSelectedNeighborsImageFunction();
  ~LinearInterpolateSelectedNeighborsImageFunction() override;
  void PrintSelf(std::ostream & os, Indent indent) const override;

private:
  NeighborCheckFunctionType m_NeighborCheck = NeighborCheckFunctionType();
  /** Evaluate interpolator at image index position. */
  virtual inline OutputType EvaluateUnoptimized(
    const ContinuousIndexType & index) const;

  /** \brief A method to generically set all components to zero
   */
  template<typename RealTypeScalarRealType>
    void
    MakeZeroInitializer(const TInputImage * const inputImagePtr,
      VariableLengthVector<RealTypeScalarRealType> & tempZeros) const
      {
      // Variable length vector version to get the size of the pixel correct.
      typename TInputImage::IndexType idx;
      idx.Fill(0);
      const typename TInputImage::PixelType & tempPixel = inputImagePtr->GetPixel(idx);
      const unsigned int sizeOfVarLengthVector = tempPixel.GetSize();
      tempZeros.SetSize(sizeOfVarLengthVector);
      tempZeros.Fill(NumericTraits< RealTypeScalarRealType >::ZeroValue());
      }

  template<typename RealTypeScalarRealType>
    void
    MakeZeroInitializer(const TInputImage * const itkNotUsed( inputImagePtr ),
      RealTypeScalarRealType & tempZeros) const
      {
      // All other cases
      tempZeros = NumericTraits< RealTypeScalarRealType >::ZeroValue();
      }

};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLinearInterpolateSelectedNeighborsImageFunction.hxx"
#endif

#endif
