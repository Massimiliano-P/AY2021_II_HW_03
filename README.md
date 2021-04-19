# Electronic Technologies and Biosensors Laboratory
## Academic Year 2020/2021 - II Semester

## Assignment 03
This repository contains the PSoC Creator workspace to be used as a starting point for the successfull submission of the Assignment 03
of the course "Electronic Technologies and Biosensors Laboratory" of the II Semester of the academic year 2020/2021.

### Objective
In this Project, you have to setup an I2C Slave using the PSoC and sample 2 analog sensors using a Delta-Sigma ADC. For data visualization, you will use the Bridge Control Panel to
plot the two signals.

### Connections

Pin          | Description |
:-----------:|-------------|
2.1          | Built-in LED
2.5          | Analog Channel 0: Temperature sensor (\*)
2.6          | Analog Channel 1: LDR (photoresistor) (\**)
12.0         | I2C SCL
12.1         | I2C SDA

(*) See the connection scheme below to hook up the photoresistor (<a href="https://components101.com/asset/sites/default/files/component_datasheet/LDR%20Datasheet.pdf">Datasheet</a>)
<a href="https://ibb.co/Vm0pbTk"><img src="https://i.ibb.co/tXwPnD1/Cattura.jpg" alt="Photoresistor schematic" width="70%" border="0"></a>

(\**) Please refer to the following image and to the datasheet of the component (<a href="https://www.analog.com/media/en/technical-documentation/data-sheets/TMP35_36_37.pdf">TMP36GZ</a>) to hook up the temperature sensor

<a href="https://imgbb.com/"><img src="https://i.ibb.co/BcVt5BN/Untitled-Diagram.jpg" alt="Untitled-Diagram" border="0"></a>

### Requirements
The Project will have to meet the following requirements:
- Sample two analog sensors using a single 16-bit Delta-Sigma ADC (hint: you can use the <a href="https://www.cypress.com/file/127201/download">Analog Mux</a> for this)
- Each sample (of each channel) has to be the average of 5 consecutive samples buffered opportunely
- Set up an I2C Slave (<a href="https://www.cypress.com/file/185396/download">EZI2C</a>) to send the averaged data for the 2 channels to the Bridge Control Panel. 
  Configure the EZI2C component as follows: 100 kbps data rate, 1 address (0x08)
- The required transmission data rate is 50 Hz
- Set up a Timer with an ISR at the appropriate frequency to guarantee the data transmission rate (50 Hz) of the averaged data (5 samples)
- Control the operation of the device writing the value of the Control Register of the I2C slave (refer to the table below)
- Set up the I2C Slave buffer of the EZI2C according to the following table (set the WHO AM I to 0xBC)
- Turn on the built-in LED when the status is equal to 0b11, and turn it off otherwise (refer to 
the description of Control Register 1 for more info)

**I2C Slave Buffer Structure**

Address      | Description   | R/W |
:-----------:|---------------|:---:|
0x00         | Control Reg 1 | R/W |
0x01         | Control Reg 2 | R/W |
0x02         | Who Am I      |  R  |
0x03         | Ch0 Bit 15-8  |  R  |
0x04         | Ch0 Bit 07-0  |  R  |
0x05         | Ch1 Bit 15-8  |  R  |
0x06         | Ch1 Bit 07-0  |  R  |


**Control Register 1 Description** (Address 0x00)

Bit    | Description           | R/W | Default |
:-----:|-----------------------|:---:|:-------:|
7      | *--reserved*          |  -  |    -    |
6      | *--reserved*          |  -  |    -    |
5      | Average samples bit 3 | R/W |    0    |
4      | Average samples bit 2 | R/W |    0    |
3      | Average samples bit 1 | R/W |    0    |
2      | Average samples bit 0 | R/W |    0    |
1      | Status bit 1          | R/W |    0    |
0      | Status bit 0          | R/W |    0    |

The `status` (bits 0 and 1) can be either set to `0b00` (device stopped), 
`0b01` to sample the first channel (Ch0), `0b10` to sample the second channel
(Ch1), and finally `0b11` to sample both channels.
The register contains also the number of samples to be used for the 
computation of the average. Set the value of bits 2-5 of the Control Register 0
to meet the requirements of the project.


**Control Register 2 Description** (Address 0x01)

Bit    | Description           | R/W | Default |
:-----:|-----------------------|:---:|:-------:|
7-0    | Timer 1 Period Value  | R/W |    0    |

The Control Register 1 contains the 8-bit period value of the Timer 
used to generate the ISR required to sample the analog channels 
using the Delta-Sigma ADC. Configure this register to meet the transmission
data rate specified for this project.

**Note**: mind that the required transmission data rate (50 Hz) may differ
from the ISR frequency of the timer (because of the average computation). 
You have to set up the ISR accordingly to guarantee the required transmission data rate.


### Setup and Assignment Delivery
- One student from the team forks this repository 
- Following the forking of the repository, the other team member(s) are added as collaborators via GitHub (as you did with the previous assignments)
- Each team member clones the forked repository by entering the following command in the terminal:

    `git clone https://github.com/[your_username]/AY2021_II_HW_03.git`
- Open up the workspace in PSoC Creator
- Activate the project with your group number
- Work on your project
- In your Group folder, save the Bridge Control Panel configuration files (GROUP_XX.ini and GROUP_XX.iic) inside BRIDGE_CONTROL_PANEL_CONFIG_FILES folder  
- The last commit must be named '*GROUP_XX* Final Delivery', where XX is the group number
- One member of the team proposes a pull request before the deadline for the assignment, which is on **April 27 at 6PM**


### Evaluation
The evaluation of the assignment will take into consideration the following aspects of the project:
- successful build process without any warning/error generated
- successful programming without any error generated
- correct functioning of the device as per the requirements
- code organization (header and source files, useful comments)
- schematic organization and documentation
- successful use of git for version control (use of branches, ...)
- successful collaboration with git across group members
- successful pull request on GitHub
