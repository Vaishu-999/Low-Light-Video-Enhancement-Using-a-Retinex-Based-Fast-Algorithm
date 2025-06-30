'''

Developers: J.Pavitra(N170516)
		    A.Devaki(N170138)
		 	D.Vaishnavi(N170367)
		 	B.Yashwanth(N170612)
		 	CH.Bangaru Raju(N170599)

Date   : 13-06-2022

Algorithm : Retinex-Based Fast Algorithm for Low-Light Image 
			Enhancement 

Language : Python 3.9 

Used Technology : OpenCv

Used Library : OpenCv2

'''

#importing Required Libraries
import cv2
import math 
import matplotlib.pyplot as plt 

class RBFAlgorithm:

	#Constructor With Parameter Image path
	def __init__(self,imgpath):
		#open img and store it in class member using cv2.imread() method 
		self.imagePath=imgpath
		self.inputImage=cv2.imread(imgpath,1)# 1 for color image and 0 for black and white 
		#store the Dimensions of the image
		self.Rows,self.Columns,self.useless=self.inputImage.shape
		#converting BGR Color image to grayScale image
		self.grayImg=cv2.cvtColor(self.inputImage,cv2.COLOR_BGR2GRAY)
		#converting BGR color Image to HSV Image
		self.hsvImg=cv2.cvtColor(self.inputImage,cv2.COLOR_BGR2HSV)
		#copy the V channel of HSV for further use
		self.vChannel=self.hsvImg[:,:,2].copy()  # h s v  0 -> h , s -> 1 , v -> 2
		#copy the S Channel of HSV  for further use
		self.sChannel=self.hsvImg[:,:,1].copy()   #
		#stores the output Image
		self.outputImage=None

		self.enhancedVChannelImg=None
		self.enhancedSChannelImg=None
		#to count the frequencies of different pixel Values of V channel and S channel of HSV image
		self.vGrayFreq=[0 for i in range(256)]
		self.svGrayFreq=[0 for i in range(256)]
		'''
			calculating the frequencies of each pixel in two channels 1
		
		'''


		for i in range(self.Rows):
			for j in range(self.Columns):
				self.vGrayFreq[self.grayImg[i][j]]+=1   # here v channel
				self.svGrayFreq[self.sChannel[i][j]]+=1  # s channel frequency


		#print(self.vGrayFreq)

	#To get the maximum Gray level Value from V channel of HSV Image
	def getVmax(self):
		for pixel_value in range(255,-1,-1):
			if(self.vGrayFreq[pixel_value]!=0):
				return pixel_value
		return 0

	#to get the maximum Gray level value from S channel of HSV Image
	def getSmax(self):
		for pixel_value in range(255,-1,-1):
			if(self.svGrayFreq[pixel_value]!=0):
				return pixel_value

		return 0

	#to Get the frequency of  V channel PixelValue
	def getFrequency(self,pixelValue):
		return self.vGrayFreq[pixelValue]

	#to Get the frequency of S channel pixel Value
	def getSFrequency(self,pixelValue):
		return self.sGrayFreq[pixelValue]



	#Mean Gray Value
	def getMeanGrayValue(self):
		temp1=0
		temp2=0
		for pixelValue in range(256):
			pixelFreq=self.getFrequency(pixelValue)
			temp1+=(pixelFreq*pixelValue)
			temp2+=(pixelFreq)


		mean=temp1//temp2
		#print("Mean : " ,mean)
		return mean


	#to get constant value
	def getConstant(self):
		mean=self.getMeanGrayValue()
		temp1=0
		temp2=0
		for pixelValue in range(0,mean+1):
			freq_temp=self.getFrequency(pixelValue)
			temp1+=(freq_temp*pixelValue)
			temp2+=(freq_temp)

		temp2=128*temp2
		constantVal=temp1/temp2
		return constantVal


	def getEnlargedConstant(self):
		constantVal=self.getConstant()
		#print("Constant value : ",constantVal)
		return (1//(1+math.pow(math.e,-constantVal)))



	def getPDF(self,pixelValue):
		ans=self.getFrequency(pixelValue)
		ans=ans/(self.Rows*self.Columns)
		return ans



	def getCDF(self):
		final_cdf=0
		for pixelValue in range(0,129):
			final_cdf+=(self.getPDF(pixelValue))

		#print("cdfvalue : ",final_cdf)
		return final_cdf



	def getGamma(self):
		c1=self.getEnlargedConstant()
		cdf=self.getCDF()
		w=0.48
		ansGamma=w*(c1)+(1-w)*cdf
		return ansGamma


	def update_pixel_frequency(self):
		for i in range(256):
			self.vGrayFreq[i]=0

		for i in range(self.Rows):
			for j in range(self.Columns):
				self.vGrayFreq[self.enhancedVChannelImg[i][j]]+=1


	def applyVChannelEnhancing(self):
		vmaxValue=self.getVmax()
		self.enhancedVChannelImg=self.hsvImg[:,:,2].copy()




		# linear enhancement --------------------------------------------------------

		for i in range(self.Rows):
			for j in range(self.Columns):
				if(True):
				#linear enhancement
					self.enhancedVChannelImg[i][j]=((255//vmaxValue)*self.enhancedVChannelImg[i][j])

		#----------------------------------------------------------------------------


		#self.update_pixel_frequency();


		gamma=self.getGamma()
		#print("gamma : ",gamma)

		for i in range(self.Rows):
			for j in range(self.Columns):
				if(True):


					#------------------------------------------------------------------#

					#linear enhancement
					#self.enhancedVChannelImg[i][j]=((255//vmaxValue)*self.enhancedVChannelImg[i][j])

					#------------------------------------------------------------------#

					#gamma correction value
					oldvalue=self.enhancedVChannelImg[i][j]
					oldvalue=oldvalue/255
					temp=math.pow(oldvalue,gamma)
					temp=int(temp*255)
					self.enhancedVChannelImg[i][j]=temp

					#-------------------------------------------------------------------#

					#VE = exp(log(V) - log(VL)

					change1=self.enhancedVChannelImg[i][j]/255
					change2=self.vChannel[i][j]/255
					if change1!=0:
						change1=math.log(change1)
						#print("change1=",change1)
					if change2!=0:
						change2=math.log(change2)
					if(change1!=0):
						self.enhancedVChannelImg[i][j]=int(255*math.pow(math.e,(change2-change1)))



					#--------------------------------------------------------------------#



					temp1=self.enhancedVChannelImg[i][j]

					if(self.enhancedVChannelImg[i][j]<100):
						temp1=temp1/255
						temp1=2.5*math.pow(temp1,2)
						temp1=temp1*255
						self.enhancedVChannelImg[i][j]=int(temp1)


					#---------------------------------------------------------------------#

		#print(self.enhancedVChannelImg[-1,-1])

	def getVE_Mean(self):
		ve_freq=[0 for i in range(256)]
		for i in range(self.Rows):
			for j in range(self.Columns):
				ve_freq[self.enhancedVChannelImg[i][j]]+=1

		ve_mean=0
		for pixelValue in range(256):
			ve_mean+=(ve_freq[pixelValue]*pixelValue)
		ve_mean=ve_mean/(self.Rows*self.Columns)
		return ve_mean




	def getSMean(self):
		smean=0
		for pixelValue in range(256):
			smean+=(self.svGrayFreq[pixelValue]*pixelValue)

		smean=smean/(self.Rows*self.Columns)
		return smean


	def getVES(self):
		ve_mean=self.getVE_Mean()
		#print("vemean==",ve_mean);
		smean=self.getSMean()
		#print("smean",smean);
		return ve_mean-smean

	def applySChannelEnhancing(self):
		n=0
		ves=self.getVES()
		if ves>=0:
			n=1

		#print(ves)

		self.enhancedSChannelImg=self.sChannel.copy()
		for i in range(self.Rows):
			for j in range(self.Columns):
				tempvalue=self.sChannel[i][j]/255
				ves=ves/255
				tempPow=1+math.pow(-1,2-n)*(math.pow(abs(ves),2)+abs(ves))
				tempvalue=math.pow(tempvalue,tempPow)
				tempvalue=int(tempvalue*255)
				self.enhancedSChannelImg[i][j]=tempvalue



	def showOutput(self):
		cv2.imshow("output",self.outputImage)
		cv2.waitKey(0)
		cv2.destroyAllWindows()



	def plotImages(self):
		fig,axs=plt.subplots(2,2)
		axs[0,0].imshow(cv2.cvtColor(self.inputImage, cv2.COLOR_BGR2RGB))
		axs[0, 0].set_title('Original_Image')
		axs[0, 1].set_title('Enhanced_Image')
		axs[0,1].imshow(cv2.cvtColor(self.outputImage, cv2.COLOR_BGR2RGB))
		axs[1,0].hist(self.vChannel.flatten(),bins=256,range=(0,256))
		axs[1, 0].set_title('Original V channel ')
		axs[1, 1].set_title('Enhanced V channel ')
		axs[1,1].hist(self.enhancedVChannelImg.flatten(),bins=256,range=(0,256))
		plt.show()


	def Enhance(self):
		self.applyVChannelEnhancing()
		self.applySChannelEnhancing()
		self.hsvImg[:,:,2]=self.enhancedVChannelImg
		self.hsvImg[:,:,1]=self.enhancedSChannelImg
		self.outputImage=cv2.cvtColor(self.hsvImg,cv2.COLOR_HSV2BGR)
		#self.plotImages()


	def saveOutput(self,outputName):
		cv2.imwrite(outputName,self.outputImage)






#obj1=RBFAlgorithm('INPUTS//input1.jpg')


#obj1.Enhance()
#obj1.showOutput()
#obj1.saveOutput('OUTPUTS//output1.jpg')






