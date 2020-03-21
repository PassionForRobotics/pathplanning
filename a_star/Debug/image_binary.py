import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('./snazzy-image-2.png',0)
ret,thresh1 = cv2.threshold(img,150,200,cv2.THRESH_BINARY)
ret,thresh2 = cv2.threshold(img,127,255,cv2.THRESH_BINARY_INV)
ret,thresh3 = cv2.threshold(img,127,255,cv2.THRESH_TRUNC)
ret,thresh4 = cv2.threshold(img,250,255,cv2.THRESH_TOZERO)
ret,thresh5 = cv2.threshold(img,250,255,cv2.THRESH_TOZERO_INV)

titles = ['Original Image','BINARY','BINARY_INV','TRUNC','TOZERO','TOZERO_INV']
images = [img, thresh1, thresh2, thresh3, thresh4, thresh5]

#for i in xrange(6):
#    plt.subplot(2,3,i+1),plt.imshow(images[i],'gray')
#    plt.title(titles[i])
#    plt.xticks([]),plt.yticks([])
#
#plt.show()

cv2.imwrite('./binary-2.bmp',thresh4);

s = '#ifndef IMGAE__H\n#define IMAGE__H\n\n#define IMAGE_WIDTH ('
s += str(thresh4.shape[0])
s += ')\n#define IMAGE_HEIGHT ('
s += str(thresh4.shape[1])
s += ')\n\nunsigned char image [IMAGE_WIDTH][IMAGE_WIDTH]={'	
# access pixels and print for C array
print thresh4.shape[0], thresh4.shape[1]
for i in range(0,thresh4.shape[0]):
	s += '{'
	for j in range(0,thresh4.shape[1]):
		if thresh4[i,j]>250 :
			s += '1'
		else:
			s += '0'
		
		if j==(thresh4.shape[1]-1): #or j==(thresh4.shape[1]):
			s += ''
		else:
			s+= ','
		#print s
	if i==(thresh4.shape[0]-1):
		s += '}\n'
	else:
		s += '},\n'
		
s += '};\n#endif'	
file1 = open("../image2.h","w") 
print '../image2.h overwritten'
file1.write(s) 
file1.close()
#print s
