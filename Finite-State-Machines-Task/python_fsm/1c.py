import os
import sys
from transitions import Machine
from transitions.extensions import GraphMachine

class AbbaFSM(object):
    # Define states
    states = [
        {'name': 'start'},
        {'name': 'found_a'},
        {'name': 'found_ab'},
        {'name': 'found_abb'},
        {'name': 'found_abba', 'final': True},  # Accepting state
    ]
    
    def __init__(self):
        self.machine = GraphMachine(
            model=self,
            states=AbbaFSM.states,
            initial='start',
            auto_transitions=False,
            title="ABBA Substring Detector",
            show_conditions=True
        )
        self._setup_transitions()
    
    def _setup_transitions(self):
        # From start state
        self.machine.add_transition('a', 'start', 'found_a')
        self.machine.add_transition('b', 'start', 'start')
        self.machine.add_transition('other', 'start', 'start')
        
        # From found_a state
        self.machine.add_transition('a', 'found_a', 'found_a')
        self.machine.add_transition('b', 'found_a', 'found_ab')
        self.machine.add_transition('other', 'found_a', 'start')
        
        # From found_ab state
        self.machine.add_transition('a', 'found_ab', 'found_a')
        self.machine.add_transition('b', 'found_ab', 'found_abb')
        self.machine.add_transition('other', 'found_ab', 'start')
        
        # From found_abb state
        self.machine.add_transition('a', 'found_abb', 'found_abba')
        self.machine.add_transition('b', 'found_abb', 'start')
        self.machine.add_transition('other', 'found_abb', 'start')
        
        # From found_abba state (accepting state - stay here)
        self.machine.add_transition('a', 'found_abba', 'found_abba')
        self.machine.add_transition('b', 'found_abba', 'found_abba')
        self.machine.add_transition('other', 'found_abba', 'found_abba')
    
    def process_string(self, input_string):
        self.state = 'start'
        for char in input_string:
            if char == 'a':
                self.a()
            elif char == 'b':
                self.b()
            else:
                self.other()
            if self.state == 'found_abba':
                break
        return self.state == 'found_abba'
    
    def visualize(self):
        dot_graph = self.get_graph()

        for i, line in enumerate(dot_graph.body):
            if 'found_abba' in line and 'label=' in line:
                dot_graph.body[i] = line.replace('shape=ellipse', 'shape=doublecircle')
    
        output_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        output_png = os.path.join(output_dir, 'fsm_visualization.png')
        output_dot = os.path.join(output_dir, 'fsm_visualization.dot')
    
        os.makedirs(output_dir, exist_ok=True)
    
        with open(output_dot, 'w') as f:
            f.write(dot_graph.source)
    
        try:
            dot_graph.render(filename=os.path.splitext(output_dot)[0], format='png', cleanup=True)
            print(f"FSM visualization saved to:")
            print(f"- PNG: {output_png}")
            print(f"- DOT: {output_dot}")
        except Exception as e:
            print(f"Error generating PNG: {e}")
            print(f"DOT file still saved to: {output_dot}")
        return output_dot


def main():
    fsm = AbbaFSM()

    if len(sys.argv) > 1 and "--visualize" in sys.argv:
        fsm.visualize()
        return
    
    print("Check for 'abba' substring in a string\n")
    
    user_input = input("Enter a string: ").strip()
    
    if user_input.lower() == 'visualize':
        fsm.visualize()
    else:
        result = fsm.process_string(user_input)
        print(f"Result: {'ACCEPTED' if result else 'REJECTED'}")
        print(f"Final state: {fsm.state}")

if __name__ == "__main__":
    main()
