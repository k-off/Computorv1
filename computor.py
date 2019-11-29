import  sys
from    math import sqrt

class computor :

    def __init__ (self) :
        self.cba = [0] * 1000
        self.degree = 0
        self.discr = 0.0

    def get_input(self) :
        check = False

        if len(sys.argv) == 2 :
            self.input = sys.argv[1]
            if (len(self.input) >= 5) :
                check = self.parse_input()
            if (check == False) :
                print ("Error: input is not a valid expression.")
                sys.exit()
        elif len(sys.argv) != 2 :
            print ("Warning: exactly one argument is required.")
            
            while check == False :
                self.cba = [0] * 1000
                self.input = input("Enter an expression: ")
                if (len(self.input) >= 5) :
                    check = self.parse_input()
                else :
                    print ("Error: input is not a valid expression. Try again.")
		
			

    def parse_input(self) :

        cur_value = 0.0
        val_is_set = 0
        cur_power = 0.0
        sign = 1
        invert_sign = 1
        tmp = 0

        if (set(self.input).issubset("0123456789xX+-\*/\^= .") == False) :
            print ("Error: allowed characters set: `0123456789xX+-\*/\^= .`")
            return (False)
        
        while (len(self.input) > 0) :

            if (self.input[0] in ' *') :
                self.input = self.input[1:]
                if (invert_sign < 0 and len(self.input) < 1) :
                    print ("Error: equation must not end with a space")
                    return (False)
            elif (self.input[0] == '=') :
                self.cba[int(cur_power)] += (cur_value * sign * invert_sign)
                val_is_set = 0
                cur_power = 0
                cur_value = 0
                sign = 1
                invert_sign = -1
                self.input = self.input[1:]
                if len(self.input) < 1 :
                    print ("Error: nothing found after `=`")
                    return (False)
            elif (self.input[0] == '+') :
                self.cba[int(cur_power)] += (cur_value * sign * invert_sign)
                val_is_set = 0
                cur_power = 0
                cur_value = 0
                sign = 1
                self.input = self.input[1:]
            elif (self.input[0] == '-') :
                if (self.input[1] == ' ') :
                    self.cba[int(cur_power)] += (cur_value * sign * invert_sign)
                    val_is_set = 0
                    cur_power = 0
                    cur_value = 0
                    sign = 1
                sign *= -1
                self.input = self.input[1:]
            elif (self.input[0] in 'xX') :
                if (val_is_set == 0) :
                    cur_value = 1
                    val_is_set = 1
                cur_power = 1
                self.input = self.input[1:]
            elif (self.input[0] == '^') :
                if (cur_power != 1) :
                    print ("Error: expression not well formatted: '^^'")
                    return (False)
                self.input = self.input[1:]
            elif (self.input[0] in "0123456789.") :
                i = 0
                while (i < len(self.input) and self.input[i] in "0123456789.") : 
                    i += 1
                if (i < len(self.input)) :
                    tmp = float(self.input[0:i])
                    self.input = self.input[i:]
                else :
                    tmp = float(self.input)
                    self.input = ""
                if (cur_power == 1) :
                    cur_power = tmp
                    if (cur_power != int(cur_power) or cur_power < 0 or cur_power > 999) :
                        print ("Error: power must be integer 0 .. 1000. (%.2f - invalid)" %cur_power)
                        return (False)
                else :
                    cur_value = tmp
                    val_is_set = 1
            if len(self.input) == 0 :
                self.cba[int(cur_power)] += (cur_value * sign * invert_sign)

        counter = 0
        for i in self.cba :
            if (i != 0 and counter > 2) :
                self.display_reduced()
                self.display_degree()
                print ("The polynomial degree is stricly greater than 2, I can't solve.")
                sys.exit()
            counter += 1

        if invert_sign > 0 :
            print ("Error: no right part of equation was found")
            return (False)
    
        return (True)

    def display_reduced(self) :
        i = 0

        sign = ""
        print ("Reduced form:", end=" ")
        while (i < 1000) :
            if (self.cba[i] != 0) :
                if (i > 0 and self.cba[i] >= 0) :
                    sign = "+"
                elif (i > 0 and self.cba[i] < 0) :
                    sign = "-"
                    self.cba[i] = -self.cba[i]
                if (i == 0) :
                    print (self.cba[i], "* X ^", i, end=" ")
                else :
                    print (sign, self.cba[i], "* X ^", i, end=" ")
                if (sign == "-") :
                    self.cba[i] = -self.cba[i]
            i += 1
        if (self.cba[0] == 0 and self.cba[1] == 0 and self.cba[2] == 0) :
            print ("0 * X ^ 0 = 0")
        else :
            print ("= 0")
    
    def display_natural(self) :
        i = 0

        sign = ""
        print ("Natural form:", end=" ")       
        while (i < 1000) :
            if (self.cba[i] != 0) :
                if (i > 0 and self.cba[i] > 0) :
                    sign = "+"
                elif (i > 0 and self.cba[i] < 0) :
                    sign = "-"
                    self.cba[i] = -self.cba[i]
                if (i == 0) :
                    print (self.cba[i],  end=" ")
                elif (i == 1) :
                    if (self.cba[i] == 1) :
                        print (sign, "X", end=" ")
                    else :
                        print (sign, self.cba[i], "* X", end=" ")
                else :
                    if (self.cba[i] == 1) :
                        print (sign, "X ^", i, end=" ")
                    else :
                        print (sign, self.cba[i], "* X ^", i, end=" ")
                if (sign == "-") :
                    self.cba[i] = -self.cba[i]
            i += 1
        if (self.cba[0] == 0 and self.cba[1] == 0 and self.cba[2] == 0) :
            print ("0 = 0")
        else :
            print ("= 0")

    def display_degree(self) :
        i = 999

        while (i >= 0) :
            self.degree = i
            if self.cba[i] != 0 :
                print ("Polynomial degree:", self.degree)
                return
            i -= 1
        print ("Polynomial degree:", self.degree)
        
    def solve_and_print(self) :
        if (self.degree == 0 and self.cba[0] == 0) :
            print ("The solution is:\n\tany real number")
        elif (self.degree == 0 and self.cba[0] != 0) :
            print ("The solution is:\n\tno solutions")
        elif (self.degree == 1 and self.cba[0] == 0) :
            print ("The solution is:\n\t%f " %(self.cba[0]))
        elif (self.degree == 1 and self.cba[0] != 0) :
            print ("The solution is:\n\t%f " %(self.cba[1] / -self.cba[0]))
        elif (self.degree == 2) :
            self.discr = self.cba[1] * self.cba[1] - 4 * \
                self.cba[2] * self.cba[0]
            print ("Discriminant (b^2 - 4ac):")
            print ("\t%.2f * %.2f - 4 * %.2f * %.2f" \
                %(self.cba[1], self.cba[1], self.cba[2], self.cba[0]))
            print ("\t%.2f -  %.2f" \
                %(self.cba[1] * self.cba[1], 4 *self.cba[2] * self.cba[0]))
            print ("\t%.2f" %self.discr)
            if (self.discr < 0) :
                print ("Discriminant is strictly negative. No real solutions.")
            elif (self.discr == 0) :
                print ("Discriminant is null.")
                print ("There is a single solution (X = -b / 2a):")
                print ("\tX = %.2f / (2 * %.2f)" %(-self.cba[1], self.cba[2]))
                print ("\tX = %.2f / %.2f" %(-self.cba[1], (2 * self.cba[2])))
                print ("\tX = %.2f" %(-self.cba[1] / 2 * self.cba[2]))
            elif (self.discr > 0) :
                print ("Discriminant is strictly positive.")
                print ("There are 2 solutions:")
                print ("\tX_1 = (-b - sqrt(discriminant)) / 2a")
                print ("\tX_1 = %.2f" %((-self.cba[1] - sqrt(self.discr)) / (2 * self.cba[2])))
                print ("\tX_2 = (-b + sqrt(discriminant)) / 2a")
                print ("\tX_2 = %.2f" %((-self.cba[1] + sqrt(self.discr)) / (2 * self.cba[2])))

comp = computor()
comp.get_input()
comp.display_reduced()
comp.display_natural()
comp.display_degree()
comp.solve_and_print()
