# coordinox
An SoC based pointing device done as part of mini Project.
Often,teachers and anyone doing presentations often find it irksome to edit the ppts in the middle of an ongoing presentation.It takes a good amount of time and effort to just come back to the laptop to just make a small change and then show the changes on the projected scree.
To solve this issue and make the lives of teachers easier we thought of methods that made teachers to edit the ppts by just moving their hands in the required direction on the required position on the ppts.

Initially,methods to find the pc screen coordinates corresponding to a point on the projection screen was found.
Ultrasonic sensors are used to calculate the projection screen coordinates.
Once the projection screen coordinates are found,the corresponding pc screen coordinates are calculated by scaling the projection coordinates with the X and Y scale factors respectively.

Now,to generate the required action,python library PyautoGUI module is used.
Suppose,a line has to be generated between to points.Initially the points position are calculated.Once the points are obtained,perform the dragTo() function to generate the line between the required points.
