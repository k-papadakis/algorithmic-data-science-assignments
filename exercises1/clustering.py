import math
import sys


#define a point
class Point:
    def __init__(self,x,y):
        self.x=x
        self.y=y
    
    def __repr__(self):
        return "("+str(self.x)+"," + str(self.y)+")"

    def __eq__(self,other):
        return (self.x == other.x) and (self.y == other.y)

    def getX(self):
        return self.x

    def getY(self):
        return self.y

#define a cluster
class Cluster:
    def __init__(self, *args):
        if len(args)==1 and isinstance(args[0],Point):
            self.points = []
            self.points.append(args[0])
            self.centroid = self.__getCentroid()
        elif len(args)==2 and isinstance(args[0],Point) and isinstance(args[1],Point):
            self.points = []
            self.points.append(args[0])
            self.points.append(args[1])
            self.centroid = self.__getCentroid()
        elif len(args)==2 and isinstance(args[0],Cluster) and isinstance(args[1],Cluster):
            self.points = args[0].getPoints() + args[1].getPoints()
            self.centroid = self.__getCentroid()
        else:
            raise "Wrong arguments"

    def __repr__(self):
        return "{" + ','.join([str(p) for p in self.points]) + "}"

    def __eq__(self,other):
        return self.points==other.points
    
    def __getCentroid(self):
        n = len(self.points)
        sumx = 0
        sumy = 0
        for p in self.points:
            sumx+=p.getX()
            sumy+=p.getY()
        centroid = Point(sumx/n,sumy/n)
        return centroid

    def insertPoint(self,p):
        self.points.append(p)
    
    def getPoints(self):
        return self.points
    
    def getCentroid(self):
        return self.centroid

#get distance between 2 points
def getDistance(p1,p2):
    distance= math.sqrt((p2.getX()-p1.getX())**2+(p2.getY()-p1.getY())**2)
    return distance

#get minimum distance between 2 clusters
def getMinDistance(c1,c2):
    c1 = c1.getPoints()
    c2 = c2.getPoints()
    
    if (len(c1)==0 or len(c2)==0):
        sys.exit()
    
    min = sys.maxsize
    for p1 in c1:
        for p2 in c2:
            dist = getDistance(p1,p2)
            #print(dist)
            if dist<min:
                min = dist
    return min

#get average distance between 2 clusters
def getAvgDistance(c1,c2):
    n=0
    sum=0
    c1 = c1.getPoints()
    c2 = c2.getPoints()
    
    if (len(c1)==0 or len(c2)==0):
        sys.exit()
    
    for p1 in c1:
        for p2 in c2:
            sum += getDistance(p1,p2)
            n+=1
    
    return sum/n

#get the radius of a cluster
def getRadius(c):
    max = -sys.maxsize - 1
    points = c.getPoints()

    if (len(points)==0):
        sys.exit()

    centroid = c.getCentroid()
    for p in points:
        dist = getDistance(centroid,p)
        if dist>max:
            max=dist
    return max

#get the diameter of a cluster
def getDiam(c):
    max = -sys.maxsize - 1
    points = c.getPoints()
    
    if (len(points)==0):
        sys.exit()
    
    for p1 in range(len(points)):
        for p2 in range(p1+1,len(points)):
            dist = getDistance(points[p1],points[p2])
            if dist>max:
                max = dist
    return max

#clustering using minimum/average/radius/diameter distance method
def clustering(clusters, method="min"):
    counter = 1
    while(len(clusters)>1):
        min = sys.maxsize
        for i in range(len(clusters)):
            for j in range(i+1,len(clusters)):
                if method=="avg":
                    dist = getAvgDistance(clusters[i],clusters[j])
                elif method=="rad":
                    dist = getRadius(Cluster(clusters[i],clusters[j]))
                elif method=="diam":
                    dist = getDiam(Cluster(clusters[i],clusters[j]))
                else:
                    dist = getMinDistance(clusters[i],clusters[j])
                if dist < min:
                    c1,c2 = clusters[i], clusters[j]
                    min = dist
        print(f"------------ {counter}th iteration ------------\nMerging {c1} and {c2} with distance {min}")
        clusters.append(Cluster(c1,c2))
        clusters.remove(c1)
        clusters.remove(c2)
        print(f"Current clusters:\n {clusters}\nTotal:{len(clusters)} cluster(s)\n")
        counter+=1
    return

if __name__=="__main__":
    x = [4,7,4,6,12,3,10,11,2,5,9,12]
    y = [10,10,8,8,6,4,5,4,2,2,3,3]
    method=["min","avg","rad","diam"]

    points = []
    for i in range(len(x)):
        points.append((Point(x[i],y[i])))

    clusters = []
    for p in points:
        clusters.append(Cluster(p))

    for m in method:
        print(f"------------ Method = {m} ------------\n")
        print(f"Initialized Clusters:\n{clusters}\nTotal Clusters:{len(clusters)}\n")
        finCluster = clustering(clusters.copy(),m)