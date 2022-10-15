import time
import pyautogui

class Pointer:
    def __init__(self, driver):     
        self.driver = driver
        self.projetionDimentions = [0, 0]      # Width - 0, Height - 1
        self.scaleFactors = [1, 1]    # x - 0, y - 1    screen/projection
        self.instructionQueue = []
        self.dragQueue = []
        self.screen_width, self.screen_height = pyautogui.size()
        pyautogui.FAILSAFE = False

    def getInstructionQueue(self):
        return self.instructionQueue

    def getDragQueue(self):
        return self.dragQueue

    def __setScalingFactors(self):
        self.projetionDimentions = self.driver.getProjectionDimentions() 
        self.scaleFactors = [self.projetionDimentions[0] / self.screen_width, self.projetionDimentions[1] / self.screen_height]

    def __getScaledInstrucion(self, instruction):
        xRaw = int(instruction.split(',')[0].split(':')[1][1:])
        yRaw = int(instruction.split(',')[1])

        xCalib = self.driver.getXCalibration()
        yCalib = self.driver.getYCalibration()

        x = xCalib[0] - xRaw
        y = yCalib[1] - yRaw

        xScale = int(x / self.scaleFactors[0])
        yScale = abs(self.screen_height - abs(int(y / self.scaleFactors[1])))

        return instruction.split(':')[0] + ": " + "% s" % xScale + ", " +"% s" % yScale

    def getInstruction(self):
        instruction = self.driver.getInstruction()
        # print("Scale factors: ", self.scaleFactors)
        # print("Projection: ", self.projetionDimentions)
        
        if(instruction.split(':')[0] == 'cx'):
            self.driver.setCalibration(instruction)
            self.__setScalingFactors()
            
        elif(instruction.split(':')[0] == 'cy'):
            self.driver.setCalibration(instruction)
            self.__setScalingFactors()

        elif(instruction.split(':')[0] == 'move'):
            coordinates = self.driver.getProjectionDimentions()
            if(coordinates[0] > 0 and coordinates[1] > 0):
                # if(self.__checkDuplicates(instruction, self.driver.getInstruction()) == "nodup"):
                self.instructionQueue.append(self.__getScaledInstrucion(instruction))
                time.sleep(0.01)
            
        elif(instruction.split(':')[0] == 'click'):
            # if(self.__checkDuplicates(instruction, self.driver.getInstruction()) == "nodup"):
            self.dragQueue.append(self.__getScaledInstrucion(instruction))

    # def __checkDuplicates(self, instruction1, instruction2):
    #     point1 = [int(instruction1.split(',')[0].split(':')[1][1:]), int(instruction1.split(',')[1][1:])]
    #     point2 = [int(instruction2.split(',')[0].split(':')[1][1:]), int(instruction2.split(',')[1][1:])]

    #     if((abs(point1[0] - point2[0]) > 5) or (abs(point1[1] - point2[1]) > 5)):
    #         return "dup"
    #     else:
    #         return "nodup"


    def moveCursor(self):
        instruction = self.instructionQueue.pop()        

        if(instruction.split(':')[0] == 'move'):
            x = int(instruction.split(',')[0].split(':')[1][1:])
            y = int(instruction.split(',')[1])

            pyautogui.moveTo(x, y, tween=pyautogui.easeInQuad)

    def dragCursor(self):
        instruction = self.dragQueue.pop()  

        if(instruction.split(':')[0] == 'click'):
            # self.getInstructionQueue = []
            x = int(instruction.split(',')[0].split(':')[1][1:])
            y = int(instruction.split(',')[1])

            pyautogui.dragTo(x, y)        

            
