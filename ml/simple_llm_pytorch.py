import torch
import torch.nn as nn
import torch.nn.functional as F
import math

class SimpleLLM(nn.Module):
    """
    A minimal GPT-style transformer for demonstration.
    Same architecture as our manual implementation but in PyTorch.
    """
    
    def __init__(self, vocab_size=14, embed_dim=4, num_layers=2, num_heads=1):
        super().__init__()
        
        self.embed_dim = embed_dim
        self.num_layers = num_layers
        
        # Token embeddings and position embeddings
        self.token_embedding = nn.Embedding(vocab_size, embed_dim)
        self.position_embedding = nn.Embedding(100, embed_dim)  # max 100 positions
        
        # Stack of transformer blocks
        self.transformer_blocks = nn.ModuleList([
            TransformerBlock(embed_dim, num_heads) 
            for _ in range(num_layers)
        ])
        
        # Final layer norm and output projection
        self.ln_final = nn.LayerNorm(embed_dim)
        self.lm_head = nn.Linear(embed_dim, vocab_size, bias=False)
        
        # Initialize weights
        self.apply(self._init_weights)
        
    def _init_weights(self, module):
        """Initialize weights with small random values"""
        if isinstance(module, (nn.Linear, nn.Embedding)):
            module.weight.data.normal_(mean=0.0, std=0.02)
        elif isinstance(module, nn.LayerNorm):
            module.bias.data.zero_()
            module.weight.data.fill_(1.0)
    
    def forward(self, token_ids):
        """
        Forward pass through the model.
        Args:
            token_ids: tensor of shape (batch_size, sequence_length)
        Returns:
            logits: tensor of shape (batch_size, sequence_length, vocab_size)
        """
        batch_size, seq_len = token_ids.shape
        
        # Get token and position embeddings
        tok_emb = self.token_embedding(token_ids)  # (B, T, C)
        pos = torch.arange(0, seq_len, device=token_ids.device).unsqueeze(0)  # (1, T)
        pos_emb = self.position_embedding(pos)  # (1, T, C)
        
        # Combine embeddings
        x = tok_emb + pos_emb
        
        # Pass through transformer blocks
        for block in self.transformer_blocks:
            x = block(x)
        
        # Final layer norm and projection to vocabulary
        x = self.ln_final(x)
        logits = self.lm_head(x)
        
        return logits
    
    @torch.no_grad()
    def generate(self, prompt_ids, max_new_tokens=10, temperature=1.0):
        """
        Generate text autoregressively.
        Args:
            prompt_ids: tensor of shape (1, prompt_length) with token IDs
            max_new_tokens: number of tokens to generate
            temperature: sampling temperature (higher = more random)
        Returns:
            tensor of generated token IDs
        """
        self.eval()
        generated = prompt_ids.clone()
        
        for _ in range(max_new_tokens):
            # Get predictions for current sequence
            # (only need last token's logits for next prediction)
            logits = self.forward(generated)  # (1, T, vocab_size)
            logits = logits[:, -1, :] / temperature  # (1, vocab_size)
            
            # Convert to probabilities and sample
            probs = F.softmax(logits, dim=-1)
            next_token = torch.multinomial(probs, num_samples=1)  # (1, 1)
            
            # Append to sequence
            generated = torch.cat([generated, next_token], dim=1)
            
            # Stop if we hit end token (if we had one defined)
            # if next_token.item() == eos_token_id:
            #     break
        
        return generated


class TransformerBlock(nn.Module):
    """
    A single transformer block with:
    - Multi-head self-attention
    - Layer normalization
    - Feed-forward network
    - Residual connections
    """
    
    def __init__(self, embed_dim, num_heads):
        super().__init__()
        
        # Multi-head attention
        self.attention = nn.MultiheadAttention(
            embed_dim, 
            num_heads,
            dropout=0.0,
            batch_first=True
        )
        
        # Feed-forward network
        self.mlp = nn.Sequential(
            nn.Linear(embed_dim, 4 * embed_dim),
            nn.GELU(),  # GPT uses GELU instead of ReLU
            nn.Linear(4 * embed_dim, embed_dim),
        )
        
        # Layer normalizations
        self.ln1 = nn.LayerNorm(embed_dim)
        self.ln2 = nn.LayerNorm(embed_dim)
    
    def forward(self, x):
        """
        Forward pass through the transformer block.
        Args:
            x: tensor of shape (batch_size, seq_len, embed_dim)
        """
        seq_len = x.size(1)
        
        # Create causal mask (upper triangular) to prevent looking ahead
        mask = torch.triu(torch.ones(seq_len, seq_len), diagonal=1).bool()
        mask = mask.to(x.device)
        
        # Self-attention with residual connection
        attn_out, _ = self.attention(
            self.ln1(x), self.ln1(x), self.ln1(x),
            attn_mask=mask,
            is_causal=True
        )
        x = x + attn_out
        
        # Feed-forward with residual connection
        x = x + self.mlp(self.ln2(x))
        
        return x


# ============================================================
# DEMONSTRATION
# ============================================================

def demo():
    """Demonstrate the PyTorch LLM"""
    
    print("=" * 70)
    print("PYTORCH SIMPLE LLM DEMONSTRATION")
    print("=" * 70)
    
    # Create vocabulary
    vocab = ['<PAD>', 'the', 'cat', 'sat', 'on', 'mat', 'dog', 
             'ran', 'jumped', 'sleeps', 'a', 'big', 'small', '.']
    token_to_id = {token: i for i, token in enumerate(vocab)}
    id_to_token = {i: token for token, i in token_to_id.items()}
    
    # Create model
    model = SimpleLLM(
        vocab_size=len(vocab),
        embed_dim=4,
        num_layers=2,
        num_heads=1
    )
    
    # Count parameters
    total_params = sum(p.numel() for p in model.parameters())
    print(f"\nModel created with {total_params:,} parameters")
    
    # Show model architecture
    print(f"\nModel Architecture:")
    print(model)
    
    # Tokenize a prompt
    prompt = "the cat sat"
    prompt_tokens = prompt.lower().split()
    prompt_ids = torch.tensor([[token_to_id.get(t, 0) for t in prompt_tokens]])
    
    print(f"\nPrompt: '{prompt}'")
    print(f"Token IDs: {prompt_ids.tolist()[0]}")
    
    # Generate text
    print(f"\nGenerating text...")
    generated_ids = model.generate(prompt_ids, max_new_tokens=5, temperature=0.8)
    
    # Decode back to text
    generated_tokens = [id_to_token[id.item()] for id in generated_ids[0]]
    generated_text = ' '.join(generated_tokens)
    
    print(f"Generated: '{generated_text}'")
    
    # Show what a forward pass looks like
    print("\n" + "=" * 70)
    print("FORWARD PASS DETAILS")
    print("=" * 70)
    
    with torch.no_grad():
        # Get logits for the prompt
        logits = model(prompt_ids)  # (1, 3, 14)
        
        print(f"\nInput shape: {prompt_ids.shape} (batch_size=1, seq_len=3)")
        print(f"Output shape: {logits.shape} (batch_size=1, seq_len=3, vocab_size=14)")
        
        # Look at predictions for the last token
        last_logits = logits[0, -1, :]  # (14,)
        probs = F.softmax(last_logits, dim=-1)
        
        # Show top 5 predictions
        top_probs, top_indices = torch.topk(probs, 5)
        print(f"\nTop 5 predictions after '{prompt}':")
        for prob, idx in zip(top_probs, top_indices):
            token = id_to_token[idx.item()]
            print(f"  '{token}': {prob.item():.3f}")
    
    print("\n" + "=" * 70)
    print("KEY DIFFERENCES FROM MANUAL IMPLEMENTATION:")
    print("=" * 70)
    print("""
    1. Matrix operations are automatic (@ operator, nn.Linear)
    2. Attention mask creation is one line
    3. Layer normalization is built-in
    4. Gradient computation is automatic (with .backward())
    5. Parameter initialization is standardized
    6. Batching is handled automatically
    7. GPU acceleration with .cuda() if available
    
    The core concepts are identical - just much cleaner code!
    """)

# Run the demo
if __name__ == "__main__":
    demo()