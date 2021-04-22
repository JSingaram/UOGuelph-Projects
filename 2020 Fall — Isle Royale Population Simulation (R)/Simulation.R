step <- 6
tmax <- step * 40
Year <- 1:(tmax/step)
numruns <- 1700
MaxAdded <- 150
WolvesAdded <- 1:MaxAdded

#Moose parameters
rmax <- (1.2137/step)
K <- 1480
Mstdev <- 0.210/step

#Wolf Parameters
rsick <- -0.09571/step
rhealthy <- 0.04105/step
Wstdev <- 0.2650/step
r <- rhealthy

chealthy <- 0.274/step
csick <- 0.298/step
c <- chealthy

Dhealthy <- 0.310/step
Dsick <- 0.506/step
D <- Dhealthy

WolfSick <- 2




#Kstdev <- 0.055
#Kavg <- 0.748

# Kill parameters
Kstdev <- 3.72/step
Kavg <- 8.976 /step




Moose <- integer(tmax)
Wolf <- integer(tmax)
P <- numeric(tmax)
Predation <- numeric(tmax)
epsilon <-numeric(tmax)
WolfFails <- numeric(MaxAdded)
MooseFails <- numeric(MaxAdded)

DegreeOfInbreeding <- numeric(numruns)
AvgDegreeOfInbreeding <- numeric(MaxAdded)


Moose[1] <- 2060
Wolf[1] <- 10

for (i in 1:MaxAdded) {
    WolfFails[i] <- 0
    MooseFails[i] <- 0
    WolfMax <- 0
    Wolf[1] <- WolfSick + i
    for (j in 1:numruns) {
        c <- ((csick * WolfSick) + (chealthy * i)) / Wolf[1]
        D <- ((Dsick * WolfSick) + (Dhealthy * i)) / Wolf[1]
	  runFailed = FALSE
        sprintf("Running population")
	  DegreeOfInbreeding[j] <- 0
        for (t in 2:tmax) {
            epsilon[t] <- rnorm(1) * Kstdev
            P[t] = max(0, (Kavg + epsilon[t])) * (Moose[t-1] / K) * 2
            Predation[t] = P[t] * Wolf[t-1]
            epsilon[t] <- rnorm(1) * Mstdev
            Moose[t] = floor(Moose[t-1] * exp(rmax * (1 - Moose[t-1]/K) + epsilon[t]) - Predation[t])
            epsilon[t] <- rnorm(1) * Wstdev
            Wolf[t] = floor(Wolf[t-1] *  exp((c * P[t]) + epsilon[t] - D))
            if (Wolf[t] > WolfMax) {
                WolfMax = min(50, Wolf[t])
            }
            if (Moose[t] < (0.2 * K)) {
                MooseFails[i] <- MooseFails[i] + 1
		    runFailed = TRUE
                break
            }
            if ((Wolf[t] < (0.2 * WolfMax)) || ((WolfMax > 10) && (Wolf[t] < 5)) || (Wolf[t] < 1)) {
                WolfFails[i] <- WolfFails[i] + 1
		    runFailed = TRUE
                break
            }
            if (Wolf[t] < 50) {
                c <- c - ((Wolf[t] / 50) * (c - csick) / (4 * step))
                D <- D - ((Wolf[t] / 50) * (D - Dsick) / (4 * step))
            }
		
        }
	  if (runFailed == FALSE) {
            DegreeOfInbreeding[j] <- ((csick - c) / (csick - chealthy))
	  } else DegreeOfInbreeding[j] <- 0
    }
    AvgDegreeOfInbreeding[i] <- sum(DegreeOfInbreeding) / (numruns - (WolfFails[i] + MooseFails[i]))
}

Moose
Wolf
P
Predation

TotalFails <- MooseFails + WolfFails
SurvivalRate <- (numruns - TotalFails) / numruns
N <- numeric(tmax/step)
V <- numeric(tmax/step)
Kills <- numeric(tmax/step)


for (t in 1:tmax) {
    if (t%%step == 0) {
        N[t/step] = Moose[t]
        V[t/step] = Wolf[t]*10
        Kills[t/step] = Predation[t]
    }
}

#makeplot()
pdf(file = "C:/Programming/F20/CIS3130/Project/Sample Run for Populations.pdf", width = 10, height = 10)
plot(Year, N, type = 'b', ylim = c(0, 2000), main="Sample Run for Moose and Wolf Populations vs Time", xlab = "Year", ylab = "Population Size")
legend(1, 1950, legend = c("Moose Population", "Wolf Population"), col = c("blue", "red"), lty = 1:2, pch=c(16), cex=0.8)
points(Year, N, pch=16, col="blue")
lines(Year, N, lty=1, col="blue")
points(Year, V, lty=2, pch=16, col="red")
lines(Year, V, lty=3, col="red")
dev.off()



pdf(file = "C:/Programming/F20/CIS3130/Project/Fails.pdf", width = 10, height = 10)
plot(WolvesAdded, TotalFails, type = 'b', ylim = c(0, numruns), xlim = c(0, MaxAdded), main="Number of runs ending in critical endangerment of either species", xlab = "Wolves Added", ylab = "Number of Failures")
legend(0.88*MaxAdded, 0.8*numruns, legend = c("Total Fails", "Moose Fails", "Wolf Fails"), col = c("black", "blue", "red"), lty = 1:3, pch=c(15, 16, 16), cex=0.8)
points(WolvesAdded, TotalFails, pch=15, col="black")
lines(WolvesAdded, TotalFails, lty=1, col="black")
points(WolvesAdded, WolfFails, lty=2, pch=16, col="red")
lines(WolvesAdded, WolfFails, lty=3, col="red")
points(WolvesAdded, MooseFails, lty=2, pch=16, col="blue")
lines(WolvesAdded, MooseFails, lty=3, col="blue")
dev.off()

pdf(file = "C:/Programming/F20/CIS3130/Project/Survival Rate.pdf", width = 10, height = 10)
plot(WolvesAdded, SurvivalRate, type = 'b', ylim = c(0, 1), xlim = c(0, MaxAdded), main="Probability that both wolves and moose survive", xlab = "Wolves Added", ylab = "Percent Chance of Survival")
legend(0.88*MaxAdded, 0.8, legend = c("Survival Rate"), col = c("black"), lty = 1:1, pch=c(16), cex=0.8)
points(WolvesAdded, SurvivalRate, pch=16, col="black")
dev.off()

pdf(file = "C:/Programming/F20/CIS3130/Project/Degree of Inbreeding.pdf", width = 10, height = 10)
plot(WolvesAdded, AvgDegreeOfInbreeding, type = 'b', ylim = c(0, 1), xlim = c(0, MaxAdded), main="Degree of inbreeding for surviving populations", xlab = "Wolves Added", ylab = "Degree of Inbreeding")
legend(0.88*MaxAdded, 0.8, legend = c("Degree of Inbreeding"), col = c("black"), lty = 1:1, pch=c(16), cex=0.8)
points(WolvesAdded, AvgDegreeOfInbreeding, pch=16, col="black")
dev.off()

df <- data.frame("wolves added" = WolvesAdded, "wolf failures" = WolfFails, "moose failures" = MooseFails, "total failures" = TotalFails, "survival rate" = SurvivalRate)
write.csv(df, "C:/Programming/F20/CIS3130/Project/output.csv", row.names = FALSE)



MooseFails
WolfFails
TotalFails
WolfMax 

DegreeOfInbreeding
AvgDegreeOfInbreeding

