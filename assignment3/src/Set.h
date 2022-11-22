#pragma once
#include <vector>

template <typename T>
class Set {
	public:
		/**
		 * Constructor.
		 *
		 * Creates an empty Set, sets the skip list parameter q to the
		 * given value
		 *
		 * @param q		double. Represents the probability of an element
		 * 				not having 
		 */
		Set(double q) : q(q) {}

		/**
		 * Destructor.
		 *
		 * Clear all the extra structures used.
		 */
		~Set(){
			Node* elem = head[0];
			while(elem != nullptr){
				Node* next = elem->forward[0];
				delete elem;
				elem = next;
			}
		}
		
		/**
		 * Copies the given Set to a new one.
		 *
		 * @param S		Set. Set to be copied.
		 */
		Set(const Set& S) : q(q) {
		}

		/**
		 * Assignment.
		 * 
		 * @param S		Set. Set to be copied.
		 */
		Set& operator=(const Set& S){
			this.q = S.get_q();
			return this;
		}

		/**
		 * Insert element x in the set.
		 *
		 * @param x		Type depends on template. The element will be added
		 * 				to the Set.
		 */ 
		void insert(const T& x){
			if(contains(x)) return;
		
			int height = randomLevel();
			Node* new_elem = new Node(x, height);
		
			// If it is the first element in that height, add it to the head
			while(head.size() <= new_elem->height){
				head.push_back(new_elem);
				height--;
			}
		
			// It might happen that it is the fisrt one added
			if(height < 0) return;
		
		
			Node* tmp = head[height];
			while(height >= 0){
				//While for that level there is a smaller element and exist and following node, go forward
				while(tmp->data < x && tmp->forward[height]) tmp = tmp->forward[height];
		
				new_elem->forward[height] = tmp->forward[height];
				tmp->forward[height] = new_elem;
				
				if(new_elem->forward[height]) //If there is a next element
					// Set the backward pointer
					new_elem->forward[height]->backward[height] = new_elem;
				height--;
			}
		}

		/**
		 * Removes the element x from the set if present.
		 * 
		 * @param x		Type depends on template. The element will be removed
		 * 				from the set.
		 */
		void remove(const T& x){
			Node* elem = search(x);
		
			if(elem && elem->data == x){
				for(size_t lvl = 0; lvl <= elem->height; lvl++){
					if(elem->backward[lvl]){
						elem->backward[lvl]->forward[lvl] = elem->forward[lvl];
						if(elem->forward[lvl]){
							elem->forward[lvl]->backward[lvl] = elem->backward[lvl];
						}
					}else{
						//If there is no previous element, that means it is in the head
						head[lvl] = elem->forward[lvl];
						if(!head[lvl]){
							while(lvl + 1 > head.size())
								head.pop_back();
						}
					}
				}
				delete elem;
			}
		}

		/**
		 * It will return wheter the element is in the set.
		 *
		 * @param x		Type depends on template. The element will be searched
		 * 				in the set.
		 * @return 		It will return true if the element is in the set, false
		 * 				otherwise.
		 */
		bool contains(const T& x) const{
			Node* res = search(x);
			return res && res->data == x;
		}

		/**
		 * Returns the total search cost.
		 *
		 * @return		It will return the toal search cost.
		 */
		int total_search_cost() const{
			int cost = 0;
			Node* next = head[0];
			while(next != nullptr){
				int height = head.size() - 1;
				while(height > 0 && head[height]->data > next->data){
					cost++;
					height--;
				}
			
				Node* res = head[height];
				while(height >= 0){
					cost++;
					if(res->forward[height] && res->forward[height]->data <= next->data){
						res = res->forward[height];
					}else{
						height--;
					}
				}
				
				next = next->forward[0];
			}

			return cost;
		}

		/**
		 * Returns the number of pointers used, that is the sum of theheight 
		 * of all nodes in the skip list.
		 * 
		 * @return		Total number of pointers.	
		 */
		int number_pointers() const{
			Node* next = head[0];
			int size = head.size();
			while(next != nullptr){
				size += next->height;
				next = next->forward[0];
			}
			
			return size;
		}

		double get_q() const {
			return q;
		}
	private:
		struct Node{
			const T& data;
			Node** forward;
			Node** backward;
			int height;
			Node(const T& data, int height) : data(data), height(height) {
				forward = new Node*[height+1];
				backward = new Node*[height+1];
			
				for(int i = 0; i <= height; i++){
					forward[i] = nullptr;
					backward[i] = nullptr;
				}
			}

			~Node(){
				delete[] forward;
				delete[] backward;
			}
		};
		
		std::vector<Node*> head; // First element for every height

		// data members
		double q;
		
		// Internal procedures
		Node* search(const T& x) const{
			int height = head.size()-1;
			while(height > 0 && head[height]->data > x) height--;
			if(height < 0) return nullptr;

			Node* res = head[height];
			while(height >= 0){
				if(res->forward[height] && res->forward[height]->data <= x){
					res = res->forward[height];
				}else{
					height--;
				}
			}
			
			return res;
		}

		int randomLevel(){
		    static std::random_device rd;
		    static std::mt19937 gen(rd());
		    static std::uniform_real_distribution<> dis(0.0,1.0);
			
			int lvl = 0;
			while(dis(gen) > q) lvl++;
		
			return lvl;
		}
}; 
