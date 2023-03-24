(1.) Is Otsu’s method successful in thresholding all the images?
Some of the images can be thresholded successfully using Otsu's method, but not all.
The foreground and background of the motorway.png are effectively separated.
On glaucoma.jpg, Otu's method reveals that the majority of the blood vessels are incorrectly categorized, and this is the same for the optic_nerve_head.png and the fundus.tif. In my opinion, this is because all of the images of the nerves and eyes have less uniform lighting than the image of the motorway.
The lighter regions appear to be the focus of the thresholding. If the image has uneven lighting or the object of interest is not well separated from the background, Otsu's method may not produce satisfiable results.

(2.) How would you modify the thresholding algorithm to address any problems?
Otsu's approach is successful, but it is ineffective for images with poor lighting.
One solution is applying the thresholding algorithm to different parts of the image and combining the results. This resolves the uniformity issue because if we take a subsection, the lighting variations are much smaller than they are in the entire image. This can aid in resolving issues with uneven lighting or contrast throughout the image. 

Another one is before thresholding, the image quality can be improved by using techniques for image preprocessing like smoothing or contrast enhancement. While contrast enhancement techniques like histogram equalisation or contrast stretching can improve the overall contrast and make the object of interest more visible, smoothing can help to reduce noise in the image. 

The last method is Multi-level thresholding: This technique segments an image into multiple regions with various levels of intensity or colour by computing multiple threshold values as opposed to just one. When there are numerous objects of interest in the image or complex background structures that need to be segmented separately, this can be helpful.


(3.) What metrics are there for assessing the success of thresholding?
Depending on the particular application, there are different metrics for measuring the effectiveness of thresholding. Precision, recall, F1 score, and accuracy are some examples of frequently used metrics. 

F1 score is the harmonic mean of precision and recall, accuracy measures the overall accuracy of the predictions. Precision measures the proportion of true positives among all positive predictions, recall measures the proportion of true positives among all actual positives, and accuracy measures the proportion of true positives among all true positives.

