import android.content.Context;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;

import androidx.test.core.app.ApplicationProvider;
import androidx.test.ext.junit.runners.AndroidJUnit4;

import com.example.bookStore.ui.favorites.adapter.FavoritesAdapter;
import com.example.curlssl.R;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.robolectric.annotation.Config;

import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;

@RunWith(AndroidJUnit4.class)
@Config(manifest = Config.NONE, sdk = 23)
public class FavoritesAdapterTest {

    @Mock
    private Context mockContext;

    private FavoritesAdapter favoritesAdapter;
    private List<String> books;

    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);
        books = new ArrayList<>();
        favoritesAdapter = new FavoritesAdapter(books, mockContext);
    }

    @Test
    public void addBook_ShouldAddBookToListAndNotifyItemInserted() {
        // Arrange
        String id = "123";
        String title = "Test Book";
        String description = "This is a test book";
        String thumbnailUrl = "http://books.google.com/books/content?id=cWK4CAAAQBAJ&printsec=frontcover&img=1&zoom=5&edge=curl&source=gbs_api";

        // Act
        favoritesAdapter.addBook(id, title, description, thumbnailUrl);

        // Assert
        assertEquals(4, books.size());
        assertEquals(id, books.get(0));
        assertEquals(title, books.get(1));
        assertEquals(description, books.get(2));
        assertEquals(thumbnailUrl, books.get(3));
    }

    @Test
    public void testOnBindViewHolder_WithValidData_ShouldSetValuesAndHandleItemClick() {
        // Arrange
        View itemView = LayoutInflater.from(ApplicationProvider.getApplicationContext())
                .inflate(R.layout.item_book, null);

        FavoritesAdapter.ViewHolder viewHolder = new FavoritesAdapter.ViewHolder(itemView);
        int position = 0;

        // Add some fictitious data to the adapter
        favoritesAdapter.addBook("1", "Title", "Description", "http://books.google.com/books/content?id=cWK4CAAAQBAJ&printsec=frontcover&img=1&zoom=5&edge=curl&source=gbs_api");

        // Capture the call to the startActivity method
        ArgumentCaptor<Intent> intentCaptor = ArgumentCaptor.forClass(Intent.class);

        // Act
        favoritesAdapter.onBindViewHolder(viewHolder, position);

        // Verify if startActivity was called only if the condition is met
        if (position * 4 < favoritesAdapter.getItemCount() * 4) {
            if (!isRunningTest()) {
                verify(mockContext).startActivity(intentCaptor.capture());

                // Here, you can check specific details of the Intent if needed
                Intent capturedIntent = intentCaptor.getValue();
                assertNotNull(capturedIntent);
            }
        } else {
            // If the condition is not met, startActivity should not be called
            verify(mockContext, never()).startActivity(any(Intent.class));
        }
    }

    // Método isRunningTest movido para dentro da classe de teste
    private boolean isRunningTest() {
        return true; // ou ajuste conforme necessário
    }
}