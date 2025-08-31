import math
import random

class SimpleLLM:
    """
    A simplified LLM that demonstrates the complete inference process.
    This shows what happens when you give a prompt to ChatGPT/Claude/etc.
    """
    
    def __init__(self):
        # Vocabulary (tiny for demonstration)
        self.vocab = ['<PAD>', 'the', 'cat', 'sat', 'on', 'mat', 'dog', 
                      'ran', 'jumped', 'sleeps', 'a', 'big', 'small', '.']
        self.token_to_id = {token: i for i, token in enumerate(self.vocab)}
        self.id_to_token = {i: token for token, i in self.token_to_id.items()}
        self.vocab_size = len(self.vocab)
        
        # Model dimensions (tiny for demonstration)
        self.embed_dim = 8
        self.num_layers = 2
        
        # These would be loaded from the trained model file
        # For demo, using random values
        self.embedding_matrix = self._random_matrix(self.vocab_size, self.embed_dim)
        
        # Attention weights for each layer
        self.W_query = [self._random_matrix(self.embed_dim, self.embed_dim) 
                        for _ in range(self.num_layers)]
        self.W_key = [self._random_matrix(self.embed_dim, self.embed_dim) 
                      for _ in range(self.num_layers)]
        self.W_value = [self._random_matrix(self.embed_dim, self.embed_dim) 
                        for _ in range(self.num_layers)]
        
        # Feedforward weights for each layer
        self.W_ff1 = [self._random_matrix(self.embed_dim, self.embed_dim * 4) 
                      for _ in range(self.num_layers)]
        self.W_ff2 = [self._random_matrix(self.embed_dim * 4, self.embed_dim) 
                      for _ in range(self.num_layers)]
        
        # Output projection (to vocabulary size)
        self.W_output = self._random_matrix(self.embed_dim, self.vocab_size)
    
    def _random_matrix(self, rows, cols):
        """Create a random matrix (simulating loaded trained weights)"""
        return [[random.gauss(0, 0.5) for _ in range(cols)] for _ in range(rows)]
    
    def tokenize(self, text):
        """Convert text to tokens"""
        return text.lower().replace('.', ' .').split()
    
    def generate(self, prompt, max_tokens=10, temperature=0.7):
        """
        MAIN INFERENCE FUNCTION
        This is what happens when you interact with an LLM!
        """
        print("=" * 70)
        print("LLM INFERENCE: GENERATING TEXT")
        print("=" * 70)
        print(f"\nINPUT PROMPT: '{prompt}'")
        print(f"Temperature: {temperature} (higher = more random)")
        print(f"Max new tokens: {max_tokens}")
        print("\n" + "-" * 70)
        
        # Start with tokenizing the prompt
        tokens = self.tokenize(prompt)
        token_ids = [self.token_to_id.get(t, 0) for t in tokens]
        
        print(f"\nSTEP 1: TOKENIZE INPUT")
        print(f"  Tokens: {tokens}")
        print(f"  Token IDs: {token_ids}")
        
        # Generate tokens one at a time
        for gen_step in range(max_tokens):
            print(f"\n{'='*70}")
            print(f"GENERATING TOKEN {gen_step + 1}")
            print(f"{'='*70}")
            print(f"Current sequence: {tokens}")
            
            # ============================================================
            # FORWARD PASS THROUGH THE MODEL
            # ============================================================
            
            # Step 1: Convert token IDs to embeddings
            embeddings = self._get_embeddings(token_ids)
            print(f"\nStep 1: Look up embeddings for {len(token_ids)} tokens")
            
            # Step 2: Add positional encodings
            embeddings = self._add_position_embeddings(embeddings)
            print(f"Step 2: Add positional information")
            
            # Step 3: Pass through transformer layers
            hidden_states = embeddings
            for layer_idx in range(self.num_layers):
                print(f"\nStep 3.{layer_idx+1}: Transformer Layer {layer_idx+1}")
                
                # Self-attention
                hidden_states = self._attention_layer(hidden_states, layer_idx)
                print(f"  - Computed self-attention")
                
                # Feedforward
                hidden_states = self._feedforward_layer(hidden_states, layer_idx)
                print(f"  - Applied feedforward network")
            
            # Step 4: Get the final hidden state for the LAST token
            # This contains all the context information!
            last_hidden = hidden_states[-1]
            print(f"\nStep 4: Extract final hidden state of last token")
            print(f"  Hidden state: [{', '.join(f'{x:.3f}' for x in last_hidden)}]")
            
            # Step 5: Project to vocabulary size
            logits = self._matrix_vec_multiply(self.W_output, last_hidden)
            print(f"\nStep 5: Project to vocabulary size ({self.vocab_size})")
            
            # Step 6: Apply softmax to get probabilities
            probs = self._softmax_with_temperature(logits, temperature)
            print(f"\nStep 6: Convert to probabilities (softmax)")
            
            # Show top 5 most likely next tokens
            top_indices = sorted(range(len(probs)), key=lambda i: probs[i], reverse=True)[:5]
            print(f"\n  Top 5 predictions for next token:")
            for idx in top_indices:
                token = self.id_to_token[idx]
                prob = probs[idx]
                print(f"    '{token}': {prob:.3f} ({prob*100:.1f}%)")
            
            # Step 7: Sample from the distribution
            next_token_id = self._sample_from_distribution(probs)
            next_token = self.id_to_token[next_token_id]
            print(f"\n  SAMPLED: '{next_token}'")
            
            # Step 8: Add to sequence and continue
            tokens.append(next_token)
            token_ids.append(next_token_id)
            
            # Stop if we generate a period
            if next_token == '.':
                print("\n[Stopping: generated end-of-sentence]")
                break
        
        # Final result
        result = ' '.join(tokens)
        print(f"\n{'='*70}")
        print(f"FINAL GENERATED TEXT:")
        print(f"  '{result}'")
        print(f"{'='*70}")
        
        return result
    
    def _get_embeddings(self, token_ids):
        """Look up embeddings for tokens"""
        return [self.embedding_matrix[tid] for tid in token_ids]
    
    def _add_position_embeddings(self, embeddings):
        """Add positional information to embeddings"""
        result = []
        for pos, emb in enumerate(embeddings):
            # Simple sinusoidal position encoding
            pos_emb = []
            for i in range(self.embed_dim):
                if i % 2 == 0:
                    pos_emb.append(math.sin(pos / (10000 ** (i / self.embed_dim))))
                else:
                    pos_emb.append(math.cos(pos / (10000 ** ((i-1) / self.embed_dim))))
            # Add position to token embedding
            result.append([e + p for e, p in zip(emb, pos_emb)])
        return result
    
    def _attention_layer(self, hidden_states, layer_idx):
        """Simplified self-attention (single head for clarity)"""
        seq_len = len(hidden_states)
        
        # Compute Q, K, V
        queries = [self._matrix_vec_multiply(self.W_query[layer_idx], h) 
                   for h in hidden_states]
        keys = [self._matrix_vec_multiply(self.W_key[layer_idx], h) 
                for h in hidden_states]
        values = [self._matrix_vec_multiply(self.W_value[layer_idx], h) 
                  for h in hidden_states]
        
        # Compute attention for each position
        output = []
        for i in range(seq_len):
            # Compute attention scores (only look at previous tokens + current)
            scores = []
            for j in range(i + 1):  # Causal attention: only look backwards
                score = sum(q * k for q, k in zip(queries[i], keys[j]))
                scores.append(score / math.sqrt(self.embed_dim))
            
            # Softmax
            exp_scores = [math.exp(s) for s in scores]
            sum_exp = sum(exp_scores)
            weights = [e / sum_exp for e in exp_scores]
            
            # Weighted sum of values
            out = [0] * self.embed_dim
            for j in range(i + 1):
                for k in range(self.embed_dim):
                    out[k] += weights[j] * values[j][k]
            
            # Residual connection (add original input)
            out = [o + h for o, h in zip(out, hidden_states[i])]
            output.append(out)
        
        return output
    
    def _feedforward_layer(self, hidden_states, layer_idx):
        """Simple feedforward network"""
        output = []
        for h in hidden_states:
            # First linear layer + ReLU
            h1 = self._matrix_vec_multiply(self.W_ff1[layer_idx], h)
            h1 = [max(0, x) for x in h1]  # ReLU
            
            # Second linear layer
            h2 = self._matrix_vec_multiply(self.W_ff2[layer_idx], h1)
            
            # Residual connection
            out = [o + orig for o, orig in zip(h2, h)]
            output.append(out)
        
        return output
    
    def _matrix_vec_multiply(self, matrix, vector):
        """Multiply matrix by vector"""
        result = []
        for row in matrix:
            if len(row) != len(vector):
                # Handle dimension mismatch for output layer
                vector_padded = vector + [0] * (len(row) - len(vector))
                result.append(sum(r * v for r, v in zip(row, vector_padded)))
            else:
                result.append(sum(r * v for r, v in zip(row, vector)))
        return result
    
    def _softmax_with_temperature(self, logits, temperature):
        """Apply softmax with temperature scaling"""
        # Temperature scaling: lower = more deterministic, higher = more random
        scaled_logits = [l / temperature for l in logits]
        
        # Subtract max for numerical stability
        max_logit = max(scaled_logits)
        exp_logits = [math.exp(l - max_logit) for l in scaled_logits]
        sum_exp = sum(exp_logits)
        return [e / sum_exp for e in exp_logits]
    
    def _sample_from_distribution(self, probs):
        """Sample a token from probability distribution"""
        # Random sampling (could also do greedy: just take max)
        r = random.random()
        cumsum = 0
        for i, p in enumerate(probs):
            cumsum += p
            if r < cumsum:
                return i
        return len(probs) - 1


# DEMONSTRATION
print("SIMULATING LLM INFERENCE")
print("=" * 70)
print("Note: This uses random weights, so outputs won't make semantic sense.")
print("The point is to show the PROCESS of how generation works.\n")

# Create our mini LLM
model = SimpleLLM()

# Generate text!
model.generate("the cat", max_tokens=10, temperature=0.7)