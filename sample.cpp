// Tonemap hdr image
FloatImage toneMap(const FloatImage &im, float targetBase, float detailAmp, bool useBila, float sigmaRange)
{
	
    //decompose img to lumi and chromi
    FloatImage outImage, blur, detail;
	FloatImage lumi = lumiChromi(im)[0];
	FloatImage chromi = lumiChromi(im)[1];
	lumi = log10FloatImage(lumi); //convert lumi to log space


    //compute blur and detail img
	float d = max(im.sizeX(), im.sizeY()) / 50.f; //standard deviation
    
    //add bilateral/gaussian filter
	if(useBila == true){
		blur = bilateral(lumi, sigmaRange, d);
	}else{
		blur = gaussianBlur_seperable(lumi, d);
	}

	detail = lumi - blur;	

	
	//add scale factor
	blur = ( log10(targetBase) / (blur.max() - blur.min()) ) * ( blur - blur.max() );
	detail * = detailAmp;
    //compose img and convert back to linear space
	outImage = lumiChromi2rgb(exp10FloatImage(detail + blur), chromi);


	// add gamma correction back into the image right before returning
	outImage = changeGamma(outImage, 1.0f, 1.f / 2.2);
	cout << "tonemap finished" << endl;


	return outImage;
}

// convolve img with filter
FloatImage Filter::Convolve(const FloatImage &im, bool clamp) const
{
	FloatImage imFilter(im.width(), im.height(), im.channels());
	float weight, sum;

	// for every pixel in the image
	for (int x = 0; x < imFilter.width(); x++)
		for (int y = 0; y < imFilter.height(); y++)
			for (int z = 0; z < imFilter.channels(); z++)
			{
				//reset sum for every pixel in the img
				sum = 0.f;

				for (int xFilter = 0; xFilter < width; xFilter++)
					for (int yFilter = 0; yFilter < height; yFilter++)
					{
						// weighted sum accoring to the filter
						weight = operator()(xFilter, yFilter);
						sum += weight * im.smartAccessor(x - xFilter + int((width - 1.0) / 2.0),
												 y - yFilter + int((height - 1.0) / 2.0), z, clamp);

					}

				imFilter(x, y, z) = sum;
			}

	return imFilter;
}